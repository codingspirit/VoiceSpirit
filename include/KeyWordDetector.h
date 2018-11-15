#pragma once
#include <memory>
#include <mutex>
#include <unordered_set>

#include "KeyWordObserverInterface.h"
/*
 *    Abstract class for KeyWordDetector, using observer pattern
 */

namespace KeyWord {
class KeyWordDetector {
  public:
    void addKeyWordObserver(
        std::shared_ptr<KeyWordObserverInterface> keyWordObserver);
    void removeKeyWordObserver(
        std::shared_ptr<KeyWordObserverInterface> keyWordObserver);

    virtual ~KeyWordDetector() = default;

  protected:
    KeyWordDetector();
    void notifykeyWordObservers(std::string keyWord, size_t readerIndex) const;
    void notifykeyWordObservers(
        KeyWordObserverInterface::KeyWordDetectorState state) const;

  private:
    std::unordered_set<std::shared_ptr<KeyWordObserverInterface>>
        m_keyWordObservers;
    mutable std::mutex m_keyWordObserversMtx;
};
}  // namespace KeyWord
