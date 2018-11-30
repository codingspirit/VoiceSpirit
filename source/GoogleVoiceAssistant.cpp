#include "GoogleVoiceAssistant.h"
#include "BaseException.h"
#include "BasicLogger.h"

#include <fstream>
#include <sstream>

#include <unistd.h>

using namespace Utils::Logger;
using BaseClass::BaseException;

using google::assistant::embedded::v1alpha2::AssistConfig;
using google::assistant::embedded::v1alpha2::AssistResponse_EventType;
using google::assistant::embedded::v1alpha2::AudioOutConfig;
using google::assistant::embedded::v1alpha2::ScreenOutConfig;
using grpc::CallCredentials;
using grpc::Channel;

#define AUDIO_INPUT_MODE
// #define TEXT_INPUT_MODE

namespace VoiceAssistantService {

static const std::string TAG = "GoogleVoiceAssistant";

GoogleVoiceAssistant::GoogleVoiceAssistant(
    GoogleVoiceAssistantConfig&& config,
    std::unique_ptr<Audio::AudioOutputStream::Writer> writer,
    std::shared_ptr<Audio::AudioInputStream::Reader> reader)
    : m_gvaConfig{config},
      m_writer{std::move(writer)},
      m_reader{reader},
      m_isRunning{false},
      m_state{VoiceAssistantObserverInterface::VoiceAssistantState::NOT_READY} {
    init();
}

GoogleVoiceAssistant::~GoogleVoiceAssistant() {
    m_isRunning = false;
    m_thread->join();
}

void GoogleVoiceAssistant::init() {
    // init grpc
    grpc_init();

    // read credentials
    std::ifstream credentialsFile(m_gvaConfig.credentials_file_path);
    if (!credentialsFile) {
        std::string errorMsg = "Invalid credentials file path";
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

        throw BaseException(errorMsg);
    }
    std::stringstream credentials_buffer;
    credentials_buffer << credentialsFile.rdbuf();
    std::string credentials = credentials_buffer.str();
    m_callCredentials = grpc::GoogleRefreshTokenCredentials(credentials);
    if (m_callCredentials.get() == nullptr) {
        std::string errorMsg = "Invalid credentials";
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

        throw BaseException(errorMsg);
    }

    // create channel
    m_channel = createChannel(m_gvaConfig.api_endpoint);

    BasicLogger::getInstance().log(TAG, LogLevel::INFO, "Connected!");

    // create thread
    try {
        m_isRunning = true;
        m_thread = std::make_unique<std::thread>(
            &GoogleVoiceAssistant::threadLoop, this);
    } catch (const std::bad_alloc& e) {
        m_isRunning = false;
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       "Failed to allocate memory");
        throw;
    } catch (BaseException& e) {
        m_isRunning = false;
        BasicLogger::getInstance().log(
            TAG, LogLevel::ERROR,
            std::string("Initialization error:") + e.what());
        throw;
    }
}

std::shared_ptr<grpc::Channel> GoogleVoiceAssistant::createChannel(
    const std::string& host) {
    ::grpc::SslCredentialsOptions ssl_opts = {"", "", ""};
    auto credentials = grpc::SslCredentials(ssl_opts);
    std::string server = host + ":443";
    BasicLogger::getInstance().log(
        TAG, LogLevel::INFO, "gPRC: Creating a channel connect to:\n" + server);
    grpc::ChannelArguments channel_args;
    return CreateCustomChannel(server, credentials, channel_args);
}

void GoogleVoiceAssistant::threadLoop() {
    BasicLogger::getInstance().log(TAG, LogLevel::DEBUG,
                                   "*** THREAD START ***");
    std::unique_lock<std::mutex> locker(m_stateMtx);
    AssistResponse response;

#ifdef TEXT_INPUT_MODE
    std::string requestList[3] = {"what time is it", "who are you",
                                  "how is weather today"};
    int requestIndex = 0;
#else
    AssistRequest audioRequest = createRequest();
    std::vector<Audio::AudioInputStreamSize> audioData;
#endif

    while (m_isRunning) {
        m_cvStateChange.wait(locker, [this] {
            return (m_state != VoiceAssistantObserverInterface::
                                   VoiceAssistantState::IDLE) &&
                   (m_state != VoiceAssistantObserverInterface::
                                   VoiceAssistantState::NOT_READY);
        });
        // m_stateMtx is locked
        switch (m_state) {
            case VoiceAssistantObserverInterface::VoiceAssistantState::
                KEYWORD_TRIGGERED:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: KEYWORD_TRIGGERED");
                m_state = VoiceAssistantObserverInterface::VoiceAssistantState::
                    LISTENING;
                refreshClient();
                break;
            case VoiceAssistantObserverInterface::VoiceAssistantState::
                LISTENING:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: LISTENING");
#ifdef TEXT_INPUT_MODE
                if (!m_clientRW->Write(
                        createRequest(requestList[requestIndex]))) {
                    m_state = VoiceAssistantObserverInterface::
                        VoiceAssistantState::IDLE;
                    m_clientRW->Finish();
                    BasicLogger::getInstance().log(
                        TAG, LogLevel::INFO,
                        "GVA write request error, go back to IDLE");
                } else {
                    BasicLogger::getInstance().log(
                        TAG, LogLevel::INFO,
                        "GVA received request : " + requestList[requestIndex]);
                    m_state = VoiceAssistantObserverInterface::
                        VoiceAssistantState::THINKING;
                    requestIndex = requestIndex >= 2 ? 0 : requestIndex + 1;
                }
#else
                m_clientRW->Write(audioRequest);
                while (!m_clientRW->Read(&response)) {
                    audioData.resize(m_reader->getAvailableNum());
                    size_t nRead =
                        m_reader->read(audioData.data(), audioData.size());
                    if (nRead > 0) {
                        audioRequest.set_audio_in(
                            &((audioData)[0]),
                            audioData.size() *
                                sizeof(Audio::AudioInputStreamSize));
                    }
                }
                if (response.event_type() ==
                    AssistResponse_EventType::
                        AssistResponse_EventType_END_OF_UTTERANCE) {
                    m_state = VoiceAssistantObserverInterface::
                        VoiceAssistantState::THINKING;
                }
                if (response.has_audio_out()) {
                    m_state = VoiceAssistantObserverInterface::
                        VoiceAssistantState::RESPONDING;
                }
#endif

                break;
            case VoiceAssistantObserverInterface::VoiceAssistantState::THINKING:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: THINKING");
                while (m_clientRW->Read(&response)) {
                    if (response.has_audio_out()) {
                        m_state = VoiceAssistantObserverInterface::
                            VoiceAssistantState::RESPONDING;
                        break;
                    }
                }
                break;
            case VoiceAssistantObserverInterface::VoiceAssistantState::
                RESPONDING:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: RESPONDING");
                do {
                    if (response.has_audio_out()) {
                        std::vector<Audio::AudioOutputStreamSize> data;
                        data.resize(response.audio_out().audio_data().size() /
                                    2);
                        std::memcpy(&data[0],
                                    response.audio_out().audio_data().c_str(),
                                    response.audio_out().audio_data().length());
                        m_writer->write(&data[0], data.size());
                    }
                    for (int i = 0; i < response.speech_results_size(); i++) {
                        auto result = response.speech_results(i);
                        BasicLogger::getInstance().log(
                            TAG, LogLevel::INFO,
                            "GVA received request: \n" + result.transcript() +
                                "(" + std::to_string(result.stability()) + ")");
                    }
                    if (response.dialog_state_out()
                            .supplemental_display_text()
                            .size() > 0) {
                        BasicLogger::getInstance().log(
                            TAG, LogLevel::INFO,
                            "GVA response:\n" +
                                response.dialog_state_out()
                                    .supplemental_display_text());
                    }
                } while (m_clientRW->Read(&response));
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: IDLE");
                m_state =
                    VoiceAssistantObserverInterface::VoiceAssistantState::IDLE;
                m_clientRW->Finish();
                break;
            default:
                BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                               "GVA State: unknown state");
        }
    }
    BasicLogger::getInstance().log(TAG, LogLevel::DEBUG, "*** THREAD END ***");
    setState(VoiceAssistantObserverInterface::VoiceAssistantState::NOT_READY);
    notifyVoiceAssistantObservers(m_state);
}

bool GoogleVoiceAssistant::refreshClient() {
    // prepare clientcontext
    m_clientContext = std::make_unique<grpc::ClientContext>();
    m_clientContext->set_wait_for_ready(true);
    m_clientContext->set_credentials(m_callCredentials);
    m_assistantStub = EmbeddedAssistant::NewStub(m_channel);
    m_clientRW = m_assistantStub->Assist(m_clientContext.get());
}

void GoogleVoiceAssistant::setState(
    VoiceAssistantObserverInterface::VoiceAssistantState&& newState) {
    std::lock_guard<std::mutex> lock(m_stateMtx);
    m_state = newState;
}

void GoogleVoiceAssistant::onKeyWordDetected(std::string keyWord,
                                             size_t readerIndex) {
    if (keyWord == "jarvis") {
        BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                       "GVA is activied by KeyWord " + keyWord);
        setState(VoiceAssistantObserverInterface::VoiceAssistantState::
                     KEYWORD_TRIGGERED);
        m_reader->setIndex(readerIndex);
        // unblock m_thread
        m_cvStateChange.notify_one();
    }
}

void GoogleVoiceAssistant::onStateChanged(KeyWordDetectorState state) {}

AssistRequest GoogleVoiceAssistant::createRequest(
    const std::string& textRequest) {
    AssistRequest request;

    auto* assist_config = request.mutable_config();
    assist_config->mutable_dialog_state_in()->set_language_code(
        m_gvaConfig.language_code);

    assist_config->mutable_device_config()->set_device_id(
        m_gvaConfig.device_id);
    assist_config->mutable_device_config()->set_device_model_id(
        m_gvaConfig.device_model_id);

    assist_config->mutable_audio_out_config()->set_encoding(
        m_gvaConfig.output_encoding);
    assist_config->mutable_audio_out_config()->set_sample_rate_hertz(
        m_gvaConfig.output_sample_rate_hertz);
    assist_config->set_text_query(textRequest);

    assist_config->mutable_screen_out_config()->set_screen_mode(
        ScreenOutConfig::SCREEN_MODE_UNSPECIFIED);

    return request;
}

AssistRequest GoogleVoiceAssistant::createRequest() {
    AssistRequest request;

    auto* assist_config = request.mutable_config();
    assist_config->mutable_dialog_state_in()->set_language_code(
        m_gvaConfig.language_code);

    assist_config->mutable_device_config()->set_device_id(
        m_gvaConfig.device_id);
    assist_config->mutable_device_config()->set_device_model_id(
        m_gvaConfig.device_model_id);

    assist_config->mutable_audio_out_config()->set_encoding(
        m_gvaConfig.output_encoding);
    assist_config->mutable_audio_out_config()->set_sample_rate_hertz(
        m_gvaConfig.output_sample_rate_hertz);
    assist_config->mutable_audio_in_config()->set_encoding(
        m_gvaConfig.input_encoding);
    assist_config->mutable_audio_in_config()->set_sample_rate_hertz(
        m_gvaConfig.input_sample_rate_hertz);

    assist_config->mutable_screen_out_config()->set_screen_mode(
        ScreenOutConfig::SCREEN_MODE_UNSPECIFIED);

    return request;
}

}  // namespace VoiceAssistantService
