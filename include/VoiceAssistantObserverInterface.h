#pragma once
#include <string>

namespace VoiceAssistantService {
class VoiceAssistantObserverInterface {
  public:
    enum class VoiceAssistantState {
        NOT_READY = 0,
        IDLE,
        KEYWORD_TRIGGERED,
        LISTENING,
        THINKING,
        RESPONDING
    };
    virtual ~VoiceAssistantObserverInterface() = default;
    virtual void onStateChanged(VoiceAssistantState state) = 0;
};
}  // namespace VoiceAssistantService