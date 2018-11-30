#include "VoiceAssistant.h"

namespace VoiceAssistantService {

VoiceAssistant::VoiceAssistant(){}

void VoiceAssistant::addVoiceAssistantObserver(
    std::shared_ptr<VoiceAssistantObserverInterface> VoiceAssistantObserver) {
    std::lock_guard<std::mutex> lock(m_VoiceAssistantObserversMtx);
    m_VoiceAssistantObservers.insert(VoiceAssistantObserver);
}
void VoiceAssistant::removeVoiceAssistantObserver(
    std::shared_ptr<VoiceAssistantObserverInterface> VoiceAssistantObserver) {
    std::lock_guard<std::mutex> lock(m_VoiceAssistantObserversMtx);
    m_VoiceAssistantObservers.erase(VoiceAssistantObserver);
}

void VoiceAssistant::notifyVoiceAssistantObservers(
    VoiceAssistantObserverInterface::VoiceAssistantState state) const {
    std::lock_guard<std::mutex> lock(m_VoiceAssistantObserversMtx);
    for (auto VoiceAssistantObserver : m_VoiceAssistantObservers) {
        VoiceAssistantObserver->onStateChanged(state);
    }
}


}  // namespace VoiceAssistantService