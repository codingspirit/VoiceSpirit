#pragma once

#include "AudioInputStream.h"
#include "KeyWordDetector.h"
#include "PortAudioWrapper.h"
#include "SnowBoyWrapper.h"

#include <thread>

namespace KeyWord {
class SnowBoyKeyWordDetector : public KeyWordDetector {
  public:
    // config struct for different model
    struct SnowBoyModelConfig {
        std::string modelFiles;
        std::string keyWords;
        std::string sensitivity;
    };
    SnowBoyKeyWordDetector(
        std::shared_ptr<Audio::AudioInputStream::Reader> reader,
        const std::vector<SnowBoyModelConfig> configs,
        const std::string& resourceFile,
        const float audioGain,
        const bool applyFrontEnd);
    ~SnowBoyKeyWordDetector();

  private:
    std::shared_ptr<Audio::AudioInputStream::Reader> m_reader;
    std::unique_ptr<std::thread> m_detectionThread;
    std::unique_ptr<SnowBoyWrapper> m_snowBoyEngine;

    std::vector<std::string> m_keyWords;

    std::atomic<bool> m_isRunning;

    void detectionThreadLoop();
};
}  // namespace KeyWord