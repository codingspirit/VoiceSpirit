#include <chrono>
#include <cstring>

#include "SharedDataStream.h"

#pragma once

using namespace Utils::Logger;

namespace Utils {
namespace DataStructures {
template <typename T>
class SharedDataStream<T>::Writer {
  public:
    Writer(SharedDataStream<T>& sharedDataStream);
    ~Writer();
    /**
     * This function adds new data to the stream by copying it from the
     * provided buffer.
     *
     * @param buf A buffer to copy the data from.
     * @param nWrite The maximum number of @c wordSize words to copy.
     * @return The number of @c wordSize words copied, or zero if the
     * stream has closed.
     */
    size_t write(const T* buf, size_t nWrite);
    /**
     * Close the @c writer. After calling this function, @c write will
     * return 0
     */
    void close();
    /**
     * @brief Open writer.
     *
     */
    void open();

  private:
    // noncopyable
    Writer(const Writer&) = delete;
    Writer& operator=(const Writer&) = delete;
    void tell(size_t nDeleted);

    std::atomic<bool> m_isRunning;
    SharedDataStream<T>& m_sharedDataStream;
};

template <typename T>
SharedDataStream<T>::Writer::Writer(SharedDataStream<T>& sharedDataStream)
    : m_isRunning{false}, m_sharedDataStream{sharedDataStream} {
    BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::DEBUG,
                                   "Constructor called");
    open();
}

template <typename T>
SharedDataStream<T>::Writer::~Writer() {
    m_isRunning = false;
    m_sharedDataStream.m_isWriterCreated = false;
    BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::DEBUG,
                                   "Destructor called");
}

template <typename T>
size_t SharedDataStream<T>::Writer::write(const T* buf, size_t nWrite) {
    if (!m_isRunning) {
        BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::WARNING,
                                       "Writer is closed");
        return 0;
    }

    if (!m_sharedDataStream.isReady) {
        BasicLogger::getInstance().log(
            typeid(*this).name(), LogLevel::ERROR,
            "Someone trying to write data into to a unready SharedDataStream");
        return 0;
    }

    if (nullptr == buf) {
        BasicLogger::getInstance().log(
            typeid(*this).name(), LogLevel::ERROR,
            "Someone trying to write a nullptr to SharedDataStream");
        return 0;
    }

    if (nWrite == 0) {
        BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::ERROR,
                                       "write: Invalid parameter");
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_sharedDataStream.m_circularBufferMtx);
    size_t nDeleted;
    m_sharedDataStream.m_circularBuffer->pushRegion(buf, nWrite, nDeleted);
    tell(nDeleted);
    return nWrite;
}

template <typename T>
void SharedDataStream<T>::Writer::close() {
    m_isRunning = false;
}

template <typename T>
void SharedDataStream<T>::Writer::open() {
    m_isRunning = true;
}

template <typename T>
void SharedDataStream<T>::Writer::tell(size_t nDeleted) {
    for (auto reader : m_sharedDataStream.m_readers) {
        reader->updateIndex(nDeleted);
    }
}

}  // namespace DataStructures
}  // namespace Utils
