#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_set>
#include <vector>

#include "BasicLogger.h"
#include "CircularBuffer.h"

using namespace Utils::Logger;

namespace Utils {
namespace DataStructures {
/**
 *Template class for SharedDataStream, which allowed several reader but only one
 *writer. Inside this class use a CircularBuffer to store data stream.
 *
 * @tparam T Type of data which SharedDataStream stored.
 */
template <typename T>
class SharedDataStream {
  public:
    class Writer;
    class Reader;

    SharedDataStream(size_t size);
    ~SharedDataStream();
    std::unique_ptr<Writer> createWriter();
    std::shared_ptr<Reader> createReader();
    std::atomic<bool> isReady;

  private:
    // noncopyable
    SharedDataStream(const SharedDataStream&) = delete;
    SharedDataStream& operator=(const SharedDataStream&) = delete;

    std::shared_ptr<CircularBuffer<T>> m_circularBuffer;
    std::mutex m_circularBufferMtx;
    std::atomic<bool> m_isWriterCreated;
    std::unordered_set<std::shared_ptr<Reader>> m_readers;
    std::mutex m_writerReaderMtx;
};
template <typename T>
SharedDataStream<T>::SharedDataStream(size_t size)
    : isReady{false}, m_circularBuffer{nullptr}, m_isWriterCreated{false} {
    m_circularBuffer = std::make_shared<CircularBuffer<T>>(size);
    isReady = true;
}

template <typename T>
SharedDataStream<T>::~SharedDataStream() {
    isReady = false;
}

template <typename T>
std::unique_ptr<typename SharedDataStream<T>::Writer>
SharedDataStream<T>::createWriter() {
    std::lock_guard<std::mutex> lock(m_writerReaderMtx);
    if (!m_isWriterCreated) {
        m_isWriterCreated = true;
        return std::unique_ptr<Writer>(new Writer(*this));
    } else {
        BasicLogger::getInstance().log(typeid(*this).name(), LogLevel::ERROR,
                                       "Writer is already created");
        return nullptr;
    }
}
template <typename T>
std::shared_ptr<typename SharedDataStream<T>::Reader>
SharedDataStream<T>::createReader() {
    std::lock_guard<std::mutex> lock(m_writerReaderMtx);
    auto newReader = std::make_shared<Reader>(*this);
    m_readers.insert(newReader);
    return newReader;
}

}  // namespace DataStructures
}  // namespace Utils

#include "Reader.h"
#include "Writer.h"