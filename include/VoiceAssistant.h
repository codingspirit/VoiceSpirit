#pragma once
#include <memory>
#include <mutex>
#include <unordered_set>

#include "VoiceAssistantObserverInterface.h"
/*
 *    Abstract class for VoiceAssistant, using observer pattern
 */

namespace VoiceAssistantService {
class VoiceAssistant {
  public:
    void addVoiceAssistantObserver(
        std::shared_ptr<VoiceAssistantObserverInterface>
            VoiceAssistantObserver);
    void removeVoiceAssistantObserver(
        std::shared_ptr<VoiceAssistantObserverInterface>
            VoiceAssistantObserver);

    virtual ~VoiceAssistant() = default;

  protected:
    VoiceAssistant();
    void notifyVoiceAssistantObservers(
        VoiceAssistantObserverInterface::VoiceAssistantState state) const;

  private:
    std::unordered_set<std::shared_ptr<VoiceAssistantObserverInterface>>
        m_VoiceAssistantObservers;
    mutable std::mutex m_VoiceAssistantObserversMtx;
};
}  // namespace VoiceAssistantService
