#pragma once

#include "SharedDataStream.h"

namespace Utils {
namespace DataStructures {
template <typename T>
class SharedDataStream<T>::Reader {
  public:
    Reader(SharedDataStream<T>& sharedDataStream);
    ~Reader();
    /*
     * if @c nRead == 0, will read everything it can read
     */
    size_t read(T* buf, size_t nRead);
    void updateIndex(size_t nDeleted);
    size_t getAvailableNum();

  private:
    SharedDataStream<T>& m_sharedDataStream;
    std::atomic<size_t>
        m_index;  // to avoid one reader read same data twice, mark the index
                  // as last time read position + 1 sizeof(T)
};

template <typename T>
SharedDataStream<T>::Reader::Reader(SharedDataStream<T>& sharedDataStream)
    : m_sharedDataStream{sharedDataStream}, m_index{0} {
    BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::DEBUG,
                                   "Constructor called");
}

template <typename T>
SharedDataStream<T>::Reader::~Reader() {}

template <typename T>
size_t SharedDataStream<T>::Reader::read(T* buf, size_t nRead) {
    if (!m_sharedDataStream.isReady) {
        BasicLogger::getInstance().log(
            typeid(*this).name(), LogLevel::ERROR,
            "Someone trying to read data from a unready SharedDataStream");
        return 0;
    }
    if (nullptr == buf) {
        BasicLogger::getInstance().log(
            typeid(*this).name(), LogLevel::ERROR,
            "Someone trying to write to nullptr from SharedDataStream");
        return 0;
    }
    if (nRead > (m_sharedDataStream.m_circularBuffer->size())) {
        BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::ERROR,
                                       "read: required read num is too much");
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_sharedDataStream.m_circularBufferMtx);
    size_t ret = 0;

    ret = m_sharedDataStream.m_circularBuffer->getRegion(buf, m_index, nRead);
    if (ret == 0) {
        BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::ERROR,
                                       "read: read nothing");
    }
    m_index += ret;
    return ret;
}

template <typename T>
void SharedDataStream<T>::Reader::updateIndex(size_t nDeleted) {
    m_index = (m_index > nDeleted) ? (m_index - nDeleted) : 0;
}

template <typename T>
size_t SharedDataStream<T>::Reader::getAvailableNum() {
    return (m_sharedDataStream.m_circularBuffer->size() - m_index);
}

}  // namespace DataStructures
}  // namespace Utils