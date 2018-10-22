#include "Recorder.h"
#include "BaseException.h"

using namespace BaseClass;

namespace Audio {
namespace Recorder {
static const std::string TAG = "Recorder";
Recorder::Recorder(const int sampleRate,
                   const int bitsPerSample,
                   const int numChannels,
                   std::unique_ptr<AudioInputStream::Writer> writer)
    : m_writer{std::move(writer)},
      m_sampleRate{sampleRate},
      m_bitsPerSample{bitsPerSample},
      m_numChannels{numChannels},
      m_isPortAudioSetup(false) {
    try {
        initPortAudio();
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
};
Recorder::~Recorder() { m_isPortAudioSetup = false; }
void Recorder::initPortAudio() {
    m_portAudioWrapper = std::make_unique<PortAudio::PortAudioWrapper>(
        m_sampleRate, m_numChannels, m_bitsPerSample, PortAudio::IOType::INPUT,
        [this](const void* data, unsigned long size) {
            // Called by portaudio callback
            size_t writtenNum =
                m_writer->write(static_cast<const int16_t*>(data), size);

            if (writtenNum == 0) {
                BasicLogger::getInstance().log(TAG, LogLevel::WARNING,
                                               "Failed when trying to write");
            }
        });

    m_isPortAudioSetup = true;
}
void Recorder::startRecord() {
    m_writer->open();
    m_portAudioWrapper->startStream();
}
void Recorder::stopRecord() {
    m_portAudioWrapper->stopStream();
    m_writer->close();
}

}  // namespace Recorder
}  // namespace Audio
