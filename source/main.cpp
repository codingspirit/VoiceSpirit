#include <unistd.h>
#include <memory>
#include <thread>

#include "BasicLogger.h"

using namespace Utils::Logger;

int main(int argc, char const* argv[]) {
    std::string TAG = "main";
    BasicLogger::getInstance().log(TAG, LogLevel::WARNING, "hello");
    BasicLogger::getInstance().log(TAG, LogLevel::ERROR, "hello");

    auto thread1 = std::make_unique<std::thread>([]() {
        bool wtf = false;
        while (true) {
            if (wtf)
                BasicLogger::getInstance().setLogFilterLvl(LogLevel::ERROR);
            else
                BasicLogger::getInstance().setLogFilterLvl(LogLevel::WARNING);

            wtf = !wtf;
        }
    });
    auto thread3 = std::make_unique<std::thread>([]() {
        for (int i = 0; i < 100; i++) {
            BasicLogger::getInstance().log("thread3", LogLevel::ERROR,
                                           "from thread2:" + std::to_string(i));
            BasicLogger::getInstance().log("thread3", LogLevel::WARNING,
                                           "from thread2:" + std::to_string(i));
            BasicLogger::getInstance().log("thread3", LogLevel::DEBUG,
                                           "from thread2:" + std::to_string(i));
        }
    });

    while (1) {
        usleep(100);
    }
    return 0;
}
