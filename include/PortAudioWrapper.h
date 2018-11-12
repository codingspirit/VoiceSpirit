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
    INPUT = 0,  // PortAudio will have input capture
    OUTPUT      // PortAudio will have output
};

class PortAudioWrapper final {
  public:
    struct PortAudioWrapperConfig {
        int sampleRate;
        int numChannels;
        int bitsPerSample;
        IOType type;
        std::function<void(const void*, unsigned long)> inputCallbackInterface;
        std::function<void(void*, unsigned long)> outputCallbackInterface;
    };
    /**
     * @brief Construct a new Port Audio Wrapper object. Before use it,
     * @c addStream first
     *
     */
    PortAudioWrapper();
    /**
     * @brief Destroy the Port Audio Wrapper object
     *
     */
    ~PortAudioWrapper();

    /**
     * @brief add stream with @c specific config.
     *
     * @param config
     */
    void addStream(const PortAudioWrapperConfig& config);

    void startStream(const IOType& type);
    void stopStream(const IOType& type);

  private:
    /**
     * @brief Portaudio will call this function once sampling is done
     *
     * @param inputBuffer
     * @param outputBuffer
     * @param numSamples
     * @param timeInfo
     * @param statusFlags
     * @param userData
     * @return int
     */
    static int portAudioInputCallback(const void* inputBuffer,
                                      void* outputBuffer,
                                      unsigned long numSamples,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void* userData);

    static int portAudioOutputCallback(const void* inputBuffer,
                                       void* outputBuffer,
                                       unsigned long numSamples,
                                       const PaStreamCallbackTimeInfo* timeInfo,
                                       PaStreamCallbackFlags statusFlags,
                                       void* userData);

    std::function<void(const void*, unsigned long)> m_inputCallbackInterface;
    std::function<void(void*, unsigned long)> m_outputCallbackInterface;

    // stream memory will be controlled by portaudio itself, so we don't use
    // smart pointer here
    PaStream* m_paInputStream;
    PaStream* m_paOutputStream;

    std::mutex m_portAudioMtx;
};
}  // namespace PortAudio

}  // namespace Audio
