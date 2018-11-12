#include "Recorder.h"
#include "BaseException.h"

using Audio::PortAudio::PortAudioWrapper;
using BaseClass::BaseException;
using namespace Utils::Logger;

namespace Audio {
namespace Recorder {
static const std::string TAG = "Recorder";

Recorder::Recorder(
    const int sampleRate,
    const int bitsPerSample,
    const int numChannels,
    std::unique_ptr<AudioInputStream::Writer> writer,
    std::shared_ptr<PortAudio::PortAudioWrapper> portAudioWrapper)
    : m_writer{std::move(writer)},
      m_sampleRate{sampleRate},
      m_bitsPerSample{bitsPerSample},
      m_numChannels{numChannels},
      m_portAudioWrapper{portAudioWrapper},
      m_isRecording{false},
      m_isReady{false} {
    try {
        PortAudioWrapper::PortAudioWrapperConfig config;
        config.bitsPerSample = m_bitsPerSample;
        config.numChannels = m_numChannels;
        config.sampleRate = m_sampleRate;
        config.type = PortAudio::IOType::INPUT;
        config.inputCallbackInterface = [this](const void* data,
                                               unsigned long size) {
            // Called by portaudio callback
            size_t writtenNum =
                m_writer->write(static_cast<const int16_t*>(data), size);

            if (writtenNum == 0) {
                BasicLogger::getInstance().log(TAG, LogLevel::WARNING,
                                               "Failed when trying to write");
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
};

Recorder::~Recorder() {
    if (m_isRecording) stopRecord();
    m_isReady = false;
}

bool Recorder::isRecording() const { return m_isRecording; }

void Recorder::startRecord() {
    if (m_isReady) {
        if (!m_isRecording) {
            m_writer->open();
            m_portAudioWrapper->startStream(PortAudio::IOType::INPUT);
            m_isRecording = true;
        }
    } else {
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       "Recoreder is not ready yet");
    }
}

void Recorder::stopRecord() {
    if (m_isReady) {
        if (m_isRecording) {
            m_portAudioWrapper->stopStream(PortAudio::IOType::INPUT);
            m_writer->close();
            m_isRecording = false;
        }
    } else {
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       "Recoreder is not ready yet");
    }
}

}  // namespace Recorder
}  // namespace Audio
