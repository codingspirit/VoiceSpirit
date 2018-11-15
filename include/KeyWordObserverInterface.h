#pragma once
#include <string>

namespace KeyWord {
class KeyWordObserverInterface {
  public:
    enum class KeyWordDetectorState {
        ERROR = 0,
        ACTIVE,  // KeyWordDetector is active
        STOP     // KeyWordDetector is stopped
    };
    virtual ~KeyWordObserverInterface() = default;
    virtual void onKeyWordDetected(std::string keyWord, size_t readerIndex) = 0;
    virtual void onStateChanged(KeyWordDetectorState state) = 0;
};
}  // namespace KeyWord