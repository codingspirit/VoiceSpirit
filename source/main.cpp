#include <unistd.h>
#include <memory>
#include <thread>

#include "AudioStream.h"
#include "BasicLogger.h"
#include "GoogleVoiceAssistant.h"
#include "Player.h"
#include "Recorder.h"
#include "SnowBoyKeyWordDetector.h"

using namespace Utils::Logger;

int main(int argc, char const* argv[]) {
    auto inputStream = std::make_unique<Audio::AudioInputStream>(16384);
    auto ouputStream = std::make_unique<Audio::AudioOutputStream>(163840);

    auto portAudioWrapper =
        std::make_shared<Audio::PortAudio::PortAudioWrapper>();

    auto snowBoyReader = inputStream->createReader();

    auto recorder = std::make_unique<Audio::Recorder::Recorder>(
        16000, 16, 1, inputStream->createWriter(), portAudioWrapper);

    recorder->startRecord();

    std::vector<KeyWord::SnowBoyKeyWordDetector::SnowBoyModelConfig> config;
    KeyWord::SnowBoyKeyWordDetector::SnowBoyModelConfig tConfig;
    tConfig.modelFiles = "../resources/alexa.umdl";
    tConfig.keyWords = "alexa";
    tConfig.sensitivity = "0.6";
    config.push_back(tConfig);
    tConfig.modelFiles = "../resources/jarvis.umdl";
    tConfig.keyWords = "jarvis";
    tConfig.sensitivity = "0.8,0.80";
    config.push_back(tConfig);
    // tConfig.modelFiles = "../resources/OK Google.pmdl";
    // tConfig.keyWords = "ok google";
    // tConfig.sensitivity = "0.5";
    // config.push_back(tConfig);

    auto snowBoy = std::make_unique<KeyWord::SnowBoyKeyWordDetector>(
        snowBoyReader, config, "../resources/common.res", 1.0, true);

    VoiceAssistantService::GoogleVoiceAssistant::GoogleVoiceAssistantConfig
        gvaConfig;
    gvaConfig.api_endpoint = "embeddedassistant.googleapis.com";
    gvaConfig.credentials_file_path = "../resources/credentials.json";
    gvaConfig.language_code = "en-US";
    gvaConfig.device_id = "default";
    gvaConfig.device_model_id = "default";
    gvaConfig.output_sample_rate_hertz = 16000;
    gvaConfig.output_encoding =
        AudioOutConfig_Encoding::AudioOutConfig_Encoding_LINEAR16;
    gvaConfig.input_sample_rate_hertz = 16000;
    gvaConfig.input_encoding =
        AudioInConfig_Encoding::AudioInConfig_Encoding_LINEAR16;

    auto gvaPlayer = std::make_unique<Audio::Player::Player>(
        16000, 16, 1, ouputStream->createReader(), portAudioWrapper);

    auto gva = std::make_shared<VoiceAssistantService::GoogleVoiceAssistant>(
        std::move(gvaConfig), ouputStream->createWriter(),
        inputStream->createReader(), std::move(gvaPlayer));

    snowBoy->addKeyWordObserver(gva);

    while (1) {
        usleep(100000);
    }
    return 0;
}
