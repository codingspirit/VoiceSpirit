/**
 * @file GoogleVoiceAssistant.h
 * @author Alex Li (alex.li@tymphany.com)
 * @brief
 * @version 0.1
 * @date 2018-10-29
 *
 * @copyright Copyright (c) 2018
 *
 */
#pragma once

#include <condition_variable>
#include <thread>

#include "AudioStream.h"
#include "KeyWordObserverInterface.h"
#include "VoiceAssistant.h"

#include <grpc++/grpc++.h>
#include "google/assistant/embedded/v1alpha2/embedded_assistant.grpc.pb.h"
#include "google/assistant/embedded/v1alpha2/embedded_assistant.pb.h"

using google::assistant::embedded::v1alpha2::AssistRequest;
using google::assistant::embedded::v1alpha2::AssistResponse;
using google::assistant::embedded::v1alpha2::AudioInConfig_Encoding;
using google::assistant::embedded::v1alpha2::AudioOutConfig_Encoding;
using google::assistant::embedded::v1alpha2::EmbeddedAssistant;

namespace VoiceAssistantService {

class GoogleVoiceAssistant : public VoiceAssistant,
                             public KeyWord::KeyWordObserverInterface {
  public:
    struct GoogleVoiceAssistantConfig {
        std::string api_endpoint;
        std::string device_id;
        std::string device_model_id;
        std::string language_code;
        AudioOutConfig_Encoding output_encoding;
        AudioInConfig_Encoding input_encoding;
        uint16_t output_sample_rate_hertz;
        uint16_t input_sample_rate_hertz;
        std::string credentials_file_path;
    };
    /**
     * @brief Construct a new Google Voice Assistnat object.
     *
     */
    GoogleVoiceAssistant(
        GoogleVoiceAssistantConfig&& config,
        std::unique_ptr<Audio::AudioOutputStream::Writer> writer,
        std::shared_ptr<Audio::AudioInputStream::Reader> reader);
    /**
     * @brief Destroy the Google Voice Assistnat object
     *
     */
    ~GoogleVoiceAssistant();
    /**
     * @brief override KeyWordObserverInterface::onKeyWordDetected
     *
     * @param keyWord
     */
    void onKeyWordDetected(std::string keyWord, size_t readerIndex) override;
    void onStateChanged(KeyWordDetectorState state) override;

  private:
    /**
     * @brief init GVA
     *
     */
    void init();
    /**
     * @brief Create a channel to connect to Google.
     *
     * @param host domain name of a gRPC API endpoint
     * @return std::shared_ptr<grpc::Channel>
     */
    std::shared_ptr<grpc::Channel> createChannel(const std::string& host);
    /**
     * @brief thread loop used by @c m_thread
     *
     */
    void threadLoop();
    /**
     * @brief refreshClient. Call it before new request
     *
     * @return true refresh succeed
     * @return false refresh failed
     */
    bool refreshClient();
    /**
     * @brief Set the @c m_state. This method will lock @c m_stateMtx so it's
     * thread safe
     *
     * @param newState
     */
    void setState(
        VoiceAssistantObserverInterface::VoiceAssistantState&& newState);

    AssistRequest createRequest(const std::string& textRequest);
    AssistRequest createRequest(const Audio::AudioInputStreamSize*);

    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_isRunning;
    std::mutex m_stateMtx;
    VoiceAssistantObserverInterface::VoiceAssistantState m_state;
    std::condition_variable m_cvStateChange;
    GoogleVoiceAssistantConfig m_gvaConfig;
    std::shared_ptr<grpc::CallCredentials> m_callCredentials;
    std::unique_ptr<EmbeddedAssistant::Stub> m_assistantStub;
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<grpc::ClientContext> m_clientContext;
    std::unique_ptr<grpc::ClientReaderWriter<AssistRequest, AssistResponse>>
        m_clientRW;
    std::unique_ptr<Audio::AudioOutputStream::Writer> m_writer;
    std::shared_ptr<Audio::AudioInputStream::Reader> m_reader;
};
}  // namespace VoiceAssistantService