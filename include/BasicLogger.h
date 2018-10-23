#pragma once

#include <atomic>
#include <string>

#include "Singleton.h"

namespace Utils {
namespace Logger {
enum class LogLevel : unsigned int {
    DEBUG = 1,
    INFO,
    WARNING,
    ERROR,
};

class BasicLogger : public BaseClass::Singleton<BasicLogger> {
    friend class BaseClass::Singleton<BasicLogger>;

  public:
    void log(const std::string& tag,
             const LogLevel& level,
             const std::string& msg);
    void setLogFilterLvl(const LogLevel& filterLvl);

  private:
    BasicLogger();
    ~BasicLogger();

    std::string getTimeStamp();

    std::atomic<LogLevel> m_filterLvl;
};
}  // namespace Logger
}  // namespace Utils