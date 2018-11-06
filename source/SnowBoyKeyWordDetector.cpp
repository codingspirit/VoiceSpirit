#include "SnowBoyKeyWordDetector.h"
#include <sstream>
#include "BaseException.h"

#include <unistd.h>

using namespace BaseClass;

namespace KeyWord {
/// SnowBoy returns -1 if an error occurred.
static const int SNOWBOY_ERROR_DETECTION_RESULT = -1;

static const int MICROSECONDS_BETWEEN_SAMPLES = 100000;

static const std::string TAG = "SnowBoyKeyWordDetector";

SnowBoyKeyWordDetector::SnowBoyKeyWordDetector(
    std::shared_ptr<Audio::AudioInputStream::Reader> reader,
    const std::vector<SnowBoyModelConfig> configs,
    const std::string& resourceFile,
    const float audioGain,
    const bool applyFrontEnd)
    : m_reader{reader}, m_isRunning{false} {
    if (m_reader == nullptr) {
        std::string errorMsg = "Received a null reader. ";
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

        throw BaseException(errorMsg);
    }

    std::stringstream modulePaths;
    std::stringstream sensitivities;

    for (SnowBoyModelConfig c : configs) {
        if (modulePaths.str() != "") {
            modulePaths << ",";
        }
        if (sensitivities.str() != "") {
            sensitivities << ",";
        }
        modulePaths << c.modelFiles;
        sensitivities << c.sensitivity;
        m_keyWords.push_back(c.keyWords);
    }
    m_snowBoyEngine = std::make_unique<SnowBoyWrapper>(
        resourceFile.c_str(), modulePaths.str().c_str());
    m_snowBoyEngine->SetSensitivity(sensitivities.str().c_str());
    m_snowBoyEngine->SetAudioGain(audioGain);
    m_snowBoyEngine->ApplyFrontend(applyFrontEnd);

    m_isRunning = true;
    m_detectionThread = std::make_unique<std::thread>(
        &SnowBoyKeyWordDetector::detectionThreadLoop, this);
}

SnowBoyKeyWordDetector::~SnowBoyKeyWordDetector() {
    BasicLogger::getInstance().log(TAG, LogLevel::DEBUG,
                                   "*** THREAD JOINING ***");
    m_isRunning = false;
    m_detectionThread->join();
}

void SnowBoyKeyWordDetector::detectionThreadLoop() {
    BasicLogger::getInstance().log(TAG, LogLevel::DEBUG,
                                   "*** THREAD START ***");
    notifykeyWordObservers(
        KeyWordObserverInterface::KeyWordDetectorState::ACTIVE);
    std::vector<int16_t> audioData;
    while (m_isRunning) {
        audioData.resize(m_reader->getAvailableNum());
        size_t nRead = m_reader->read(audioData.data(), audioData.size());
        int detectRet = 0;
        if (0 == nRead) {
            BasicLogger::getInstance().log(TAG, LogLevel::WARNING,
                                           "reader read nothing from stream");
        } else {
            detectRet = m_snowBoyEngine->RunDetection(audioData.data(), nRead);
            if (detectRet > 0 && (detectRet <= m_keyWords.size())) {
                // detected sth.
                BasicLogger::getInstance().log(
                    TAG, LogLevel::DEBUG,
                    std::string("KeyWord detected:") +
                        m_keyWords[detectRet - 1]);
                notifykeyWordObservers(m_keyWords[detectRet - 1]);
            } else if (detectRet == SNOWBOY_ERROR_DETECTION_RESULT /*-1*/) {
                // error
                notifykeyWordObservers(
                    KeyWordObserverInterface::KeyWordDetectorState::ERROR);
            }
        }
        usleep(MICROSECONDS_BETWEEN_SAMPLES);
    }
    BasicLogger::getInstance().log(TAG, LogLevel::DEBUG, "*** THREAD END ***");
    notifykeyWordObservers(
        KeyWordObserverInterface::KeyWordDetectorState::STOP);
}

}  // namespace KeyWord