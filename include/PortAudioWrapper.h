#pragma once

#include "pa_ringbuffer.h"
#include "portaudio.h"

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace Audio {
namespace PortAudio {
enum class IOType {
    INPUT = 0,    // PortAudio will only have input capture
    OUTPUT,       // TODO: Add supoort for output
    INPUT_OUTPUT  // TODO: Add supoort for output
};

class PortAudioWrapper final {
  public:
    PortAudioWrapper(
        int sampleRate,
        int numChannels,
        int bitsPerSample,
        IOType type,
        std::function<void(const void*, unsigned long)> callbackInterface);
    ~PortAudioWrapper();

    void startStream();
    void stopStream();

  private:
    void init(int bitsPerSample, int numChannels, int sampleRate, IOType type);
    static int portAudioCallback(const void* inputBuffer,
                                 void* outputBuffer,
                                 unsigned long numSamples,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData);

    std::function<void(const void*, unsigned long)> m_callbackInterface;

    PaStream* m_paInputStream;
    PaStream* m_paOutputStream;
    int m_numLostSamples;

    std::mutex m_portAudioMtx;
};
}  // namespace PortAudio

}  // namespace Audio
