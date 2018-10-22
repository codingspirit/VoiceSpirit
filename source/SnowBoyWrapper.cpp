#undef _GLIBCXX_USE_CXX11_ABI
#define _GLIBCXX_USE_CXX11_ABI 0

/*This wrapper was added because can't build snowboy with stdc++11 or above*/

#include "SnowBoyWrapper.h"

namespace KeyWord {
SnowBoyWrapper::SnowBoyWrapper(const char* resource_name,
                               const char* model_name) {
    m_detector =
        std::make_unique<snowboy::SnowboyDetect>(resource_name, model_name);
}
SnowBoyWrapper::~SnowBoyWrapper() {}
int SnowBoyWrapper::SampleRate() const { return m_detector->SampleRate(); }
int SnowBoyWrapper::NumChannels() const { return m_detector->NumChannels(); }
int SnowBoyWrapper::BitsPerSample() const {
    return m_detector->BitsPerSample();
}

void SnowBoyWrapper::SetSensitivity(const char* sensitivity_str) {
    m_detector->SetSensitivity(sensitivity_str);
}
void SnowBoyWrapper::SetAudioGain(const float audio_gain) {
    m_detector->SetAudioGain(audio_gain);
}
void SnowBoyWrapper::ApplyFrontend(const bool apply_frontend) {
    m_detector->ApplyFrontend(apply_frontend);
}

int SnowBoyWrapper::RunDetection(const int16_t* data, int num_samples) {
    return m_detector->RunDetection(data, num_samples);
}

}  // namespace KeyWord
