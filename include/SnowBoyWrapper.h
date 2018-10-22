#pragma once

#include "snowboy-detect.h"

#include <cstdint>

namespace KeyWord {
class SnowBoyWrapper {
  public:
    SnowBoyWrapper(const char* resource_name, const char* model_name);
    ~SnowBoyWrapper();

    int SampleRate() const;
    int BitsPerSample() const;
    int NumChannels() const;

    void SetAudioGain(const float audio_gain);
    void ApplyFrontend(const bool apply_frontend);
    void SetSensitivity(const char* sensitivity_str);

    int RunDetection(const int16_t* data, int num_samples);

  private:
    std::unique_ptr<snowboy::SnowboyDetect> m_detector;
};
}  // namespace KeyWord