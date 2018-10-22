#include "KeyWordDetector.h"
#include "BasicLogger.h"

using namespace Utils::Logger;

namespace KeyWord {

KeyWordDetector::KeyWordDetector()
    : m_detectorState{KeyWordObserverInterface::KeyWordDetectorState::ERROR} {}

void KeyWordDetector::addKeyWordObserver(
    std::shared_ptr<KeyWordObserverInterface> keyWordObserver) {
    std::lock_guard<std::mutex> lock(m_keyWordObserversMtx);
    m_keyWordObservers.insert(keyWordObserver);
}
void KeyWordDetector::removeKeyWordObserver(
    std::shared_ptr<KeyWordObserverInterface> keyWordObserver) {
    std::lock_guard<std::mutex> lock(m_keyWordObserversMtx);
    m_keyWordObservers.erase(keyWordObserver);
}

void KeyWordDetector::notifykeyWordObservers(std::string keyWord) const {
    std::lock_guard<std::mutex> lock(m_keyWordObserversMtx);
    for (auto keyWordObserver : m_keyWordObservers) {
        keyWordObserver->onKeyWordDetected(keyWord);
    }
}
void KeyWordDetector::notifykeyWordObservers(
    KeyWordObserverInterface::KeyWordDetectorState state) const {
    std::lock_guard<std::mutex> lock(m_keyWordObserversMtx);
    for (auto keyWordObserver : m_keyWordObservers) {
        keyWordObserver->onStateChanged(state);
    }
}
}  // namespace KeyWord