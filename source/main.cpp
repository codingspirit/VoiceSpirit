#include <unistd.h>
#include <memory>
#include <thread>

#include "AudioInputStream.h"
#include "BasicLogger.h"
#include "GoogleVoiceAssistant.h"
#include "Recorder.h"
#include "SnowBoyKeyWordDetector.h"

using namespace Utils::Logger;
using namespace Utils::DataStructures;

int main(int argc, char const* argv[]) {
    auto stream = std::make_unique<Audio::AudioInputStream>(16384);

    auto reader = stream->createReader();

    auto recorder = std::make_unique<Audio::Recorder::Recorder>(
        16000, 16, 1, stream->createWriter());

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
        reader, config, "../resources/common.res", 1.0, true);

    VoiceAssistant::GoogleVoiceAssistant::GoogleVoiceAssistantConfig gvaConfig;
    gvaConfig.api_endpoint = "embeddedassistant.googleapis.com";
    gvaConfig.credentials_file_path = "../resources/credentials.json";
    gvaConfig.language_code = "en-US";
    gvaConfig.device_id = "default";
    gvaConfig.device_model_id = "default";
    gvaConfig.output_sample_rate_hertz = 16000;
    gvaConfig.output_encoding =
        AudioOutConfig_Encoding::AudioOutConfig_Encoding_LINEAR16;

    auto gva = std::make_shared<VoiceAssistant::GoogleVoiceAssistant>(
        std::move(gvaConfig));

    snowBoy->addKeyWordObserver(gva);

    while (1) {
        usleep(100000);
    }
    return 0;
}
