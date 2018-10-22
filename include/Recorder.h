#pragma once

#include "AudioInputStream.h"
#include "PortAudioWrapper.h"

using namespace Utils::Logger;
using namespace Utils::DataStructures;

namespace Audio {

namespace Recorder {
class Recorder {
  public:
    Recorder(const int sampleRate,
             const int bitsPerSample,
             const int numChannels,
             std::unique_ptr<AudioInputStream::Writer> writer);
    ~Recorder();
    void startRecord();
    void stopRecord();

    const int m_sampleRate;
    const int m_bitsPerSample;
    const int m_numChannels;

  private:
    // noncopyable
    Recorder(const Recorder&) = delete;
    Recorder& operator=(const Recorder&) = delete;

    void initPortAudio();

    std::unique_ptr<PortAudio::PortAudioWrapper> m_portAudioWrapper;
    std::unique_ptr<AudioInputStream::Writer> m_writer;
    std::atomic<bool> m_isPortAudioSetup;
};
}  // namespace Recorder
}  // namespace Audio