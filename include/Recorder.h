#pragma once

#include "AudioStream.h"
#include "PortAudioWrapper.h"

namespace Audio {

namespace Recorder {
class Recorder {
  public:
    Recorder(const int sampleRate,
             const int bitsPerSample,
             const int numChannels,
             std::unique_ptr<AudioInputStream::Writer> writer,
             std::shared_ptr<PortAudio::PortAudioWrapper> portAudioWrapper);
    ~Recorder();
    void startRecord();
    void stopRecord();
    bool isRecording() const;

    const int m_sampleRate;
    const int m_bitsPerSample;
    const int m_numChannels;

  private:
    // noncopyable
    Recorder(const Recorder&) = delete;
    Recorder& operator=(const Recorder&) = delete;

    std::shared_ptr<PortAudio::PortAudioWrapper> m_portAudioWrapper;
    std::unique_ptr<AudioInputStream::Writer> m_writer;
    std::atomic<bool> m_isReady;
    std::atomic<bool> m_isRecording;
};
}  // namespace Recorder
}  // namespace Audio