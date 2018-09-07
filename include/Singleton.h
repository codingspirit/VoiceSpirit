#pragma once

#include <memory>

namespace BaseClass {
template <typename T>
class Singleton {
  public:
    template <typename... Args>
    static T& getInstance(Args&&... args) noexcept {
        static T instance{std::forward<Args>(args)...};
        return instance;
    }

  protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};  // class Singleton

}  // namespace BaseClass
