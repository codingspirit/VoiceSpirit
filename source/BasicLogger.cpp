#include "BasicLogger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace Utils {
namespace Logger {
// clang-format off
#define RESET       "\033[0m"              /* Reset */
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
// clang-format on

BasicLogger::BasicLogger() : m_filterLvl{LogLevel::DEBUG} {
    this->log(typeid(*this).name(), LogLevel::INFO, __FUNCTION__);
}
BasicLogger::~BasicLogger() {
    this->log(typeid(*this).name(), LogLevel::INFO, __FUNCTION__);
}

void BasicLogger::log(const std::string& tag,
                      const LogLevel& level,
                      const std::string& msg) {
    if (static_cast<std::underlying_type<LogLevel>::type>(level) <
        static_cast<std::underlying_type<LogLevel>::type>(m_filterLvl.load()))
        return;

    std::stringstream msg_full;

    switch (level) {
        case LogLevel::DEBUG:
            msg_full << BLUE << "[DEBUG  ]";
            break;
        case LogLevel::ERROR:
            msg_full << RED << "[ERROR  ]";
            break;
        case LogLevel::INFO:
            msg_full << WHITE << "[INFO   ]";
            break;
        case LogLevel::WARNING:
            msg_full << YELLOW << "[WARNING]";
            break;
        default:
            msg_full << CYAN << "[UNKNOWN]";
            break;
    }
    msg_full << getTimeStamp() << ":"
             << "(" << tag << ") "
             << "||";
    msg_full << msg << RESET << std::endl;
    std::cout << msg_full.str();
}

void BasicLogger::setLogFilterLvl(const LogLevel& filterLvl) {
    m_filterLvl.store(filterLvl);
}

std::string BasicLogger::getTimeStamp() {
    std::stringstream timeStamp;
    using namespace std::chrono;
    auto time_now = system_clock::to_time_t(system_clock::now());
    auto time_now_ms =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    timeStamp << std::put_time(std::localtime(&time_now), "%T") << ":"
              << time_now_ms.count() % 1000;
    return timeStamp.str();
}

}  // namespace Logger
}  // namespace Utils
