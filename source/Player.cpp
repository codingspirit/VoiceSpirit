#include "Player.h"
#include "BaseException.h"

using Audio::PortAudio::PortAudioWrapper;
using BaseClass::BaseException;
using namespace Utils::Logger;

namespace Audio {
namespace Player {
static const std::string TAG = "Player";
static const size_t F_BUFFER = 115200;

Player::Player(const int sampleRate,
               const int bitsPerSample,
               const int numChannels,
               std::shared_ptr<AudioOutputStream::Reader> reader,
               std::shared_ptr<PortAudio::PortAudioWrapper> portAudioWrapper)
    : m_reader{reader},
      m_sampleRate{sampleRate},
      m_bitsPerSample{bitsPerSample},
      m_numChannels{numChannels},
      m_portAudioWrapper{portAudioWrapper},
      m_isPlaying{false},
      m_isReady{false} {
    try {
        PortAudioWrapper::PortAudioWrapperConfig config;
        config.bitsPerSample = m_bitsPerSample;
        config.numChannels = m_numChannels;
        config.sampleRate = m_sampleRate;
        config.type = PortAudio::IOType::OUTPUT;
        config.outputCallbackInterface = [this](void* data,
                                                unsigned long size) {
            // Called by portaudio callback
            std::vector<AudioOutputStreamSize> audioData;
            size_t numNeedToRead = m_reader->getAvailableNum() > size
                                       ? size
                                       : m_reader->getAvailableNum();
            audioData.resize(numNeedToRead);
            size_t readNum = m_reader->read(audioData.data(), audioData.size());
            if (readNum == 0) {
                BasicLogger::getInstance().log(
                    TAG, LogLevel::WARNING, "reader read nothing from stream");
                m_hasDataToPlay = false;
            } else {
                std::memcpy(data, audioData.data(),
                            audioData.size() * sizeof(AudioOutputStreamSize));
                m_hasDataToPlay = true;
            }
        };
        portAudioWrapper->addStream(config);
    } catch (const std::bad_alloc& e) {
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       "Failed to allocate memory");
        throw;
    } catch (BaseException& e) {
        BasicLogger::getInstance().log(
            TAG, LogLevel::ERROR,
            std::string("Initialization error:") + e.what());
        throw;
    }
    m_isReady = true;
}

Player::~Player() {
    if (m_isPlaying) stopPlay();
    m_isReady = false;
}

bool Player::isPlaying() const { return m_isPlaying; }
bool Player::hasDataToPlay() const { return m_hasDataToPlay; }

void Player::startPlay() {
    if (m_isReady) {
        if (!m_isPlaying) {
            m_portAudioWrapper->startStream(PortAudio::IOType::OUTPUT);
            m_isPlaying = true;
        }
    } else {
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       "Player is not ready yet");
    }
}

void Player::stopPlay() {
    if (m_isReady) {
        if (m_isPlaying) {
            m_portAudioWrapper->stopStream(PortAudio::IOType::OUTPUT);
            m_isPlaying = false;
        }
    } else {
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       "Player is not ready yet");
    }
}

}  // namespace Player
}  // namespace Audio