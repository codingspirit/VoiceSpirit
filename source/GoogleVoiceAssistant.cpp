#include "GoogleVoiceAssistant.h"
#include "BaseException.h"
#include "BasicLogger.h"

#include <fstream>
#include <sstream>

#include <unistd.h>

using namespace Utils::Logger;
using BaseClass::BaseException;

using google::assistant::embedded::v1alpha2::AssistConfig;
using google::assistant::embedded::v1alpha2::AudioOutConfig;
using google::assistant::embedded::v1alpha2::ScreenOutConfig;
using grpc::CallCredentials;
using grpc::Channel;

namespace VoiceAssistant {

static const std::string TAG = "GoogleVoiceAssistant";

GoogleVoiceAssistant::GoogleVoiceAssistant(GoogleVoiceAssistantConfig&& config)
    : m_gvaConfig{config}, m_isRunning{false}, m_state{GVAState::NOT_READY} {
    init();
}

GoogleVoiceAssistant::~GoogleVoiceAssistant() {
    m_isRunning = false;
    m_thread->join();
}

void GoogleVoiceAssistant::init() {
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

    // prepare clientcontext
    m_clientContext.set_wait_for_ready(true);
    m_clientContext.set_credentials(m_callCredentials);

    // create connection
    m_channel = createChannel(m_gvaConfig.api_endpoint);
    m_assistantStub = EmbeddedAssistant::NewStub(m_channel);

    m_clientRW = m_assistantStub->Assist(&m_clientContext);

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
    std::string requestList[2] = {"what time is it", "who are you"};
    int requestIndex = 0;

    while (m_isRunning) {
        m_cvStateChange.wait(locker, [this] {
            return (m_state != GVAState::IDLE) &&
                   (m_state != GVAState::NOT_READY);
        });
        // m_stateMtx is locked
        switch (m_state) {
            case GVAState::KEYWORD_TRIGGERED:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: KEYWORD_TRIGGERED");
                m_state = GVAState::LISTENING;
                break;
            case GVAState::LISTENING:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: LISTENING");

                if (!m_clientRW->Write(
                        createRequest(requestList[requestIndex]))) {
                    m_state = GVAState::IDLE;
                    BasicLogger::getInstance().log(
                        TAG, LogLevel::INFO,
                        "GVA write request error, go back to IDLE");
                } else {
                    m_state = GVAState::THINKING;
                    requestIndex = requestIndex >= 1 ? 0 : requestIndex + 1;
                }

                break;
            case GVAState::THINKING:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: THINKING");
                if (!m_clientRW->Read(&response)) {
                    m_state = GVAState::IDLE;
                } else {
                    m_state = GVAState::RESPONDING;
                }
                break;
            case GVAState::RESPONDING:
                BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                               "GVA State: RESPONDING");
                do {
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
                m_state = GVAState::IDLE;
                break;
            default:
                BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                               "GVA State: unknown state");
        }
    }
    BasicLogger::getInstance().log(TAG, LogLevel::DEBUG, "*** THREAD END ***");
}

void GoogleVoiceAssistant::setState(GVAState&& newState) {
    std::lock_guard<std::mutex> lock(m_stateMtx);
    m_state = newState;
}

void GoogleVoiceAssistant::onKeyWordDetected(std::string keyWord) {
    if (keyWord == "jarvis") {
        BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                       "GVA is activied by KeyWord " + keyWord);
        setState(GVAState::KEYWORD_TRIGGERED);
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

}  // namespace VoiceAssistant
