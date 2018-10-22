#pragma once

#include <cstddef>
#include <cstring>
#include <vector>

namespace Utils {
namespace DataStructures {
template <typename T>
class CircularBuffer {
    // CircularBuffer container based on std::vector. Iterator is implemented
    // Thread safe should be guaranteed by caller
  public:
    CircularBuffer(size_t size);
    ~CircularBuffer();

    // override []
    T& operator[](size_t i);
    T at(size_t i);
    void push_back(T element);
    void push_front(T element);
    const T& back() const;
    const T& front() const;
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;
    typename std::vector<T>::reverse_iterator rbegin();
    typename std::vector<T>::reverse_iterator rend();
    typename std::vector<T>::const_reverse_iterator rbegin() const;
    typename std::vector<T>::const_reverse_iterator rend() const;
    bool full() const;
    bool empty() const;
    void clear();
    void resize(size_t newSize);
    typename std::vector<T>::size_type size() const;
    /*
     * This function provide a more efficient way to push a continuous elements
     * region to the end of circular buffer(using memcpy instead of push_back
     * one by one)
     * @para elementsAddr start addr of elements need to push
     * @para nWrite num of elements need to push
     * @return the num of elements pushed success. return 0 if push failed
     */
    size_t pushRegion(const T* elementsAddr, size_t nWrite);
    /*
     * This overload method will take one more parameter to record if there is
     * any elements is deleted because pushRegion
     * @para elementsAddr start addr of elements need to push
     * @para nWrite num of elements need to push
     * @para nDeleted num of elements deleted
     * @return the num of elements pushed success. return 0 if push failed
     */
    size_t pushRegion(const T* elementsAddr, size_t nWrite, size_t& nDeleted);
    /*
     * This function provide a more efficient way to read a continuous elements
     * region from the specific region of circular buffer(using memcpy instead
     * of read one by one)
     * @para elementsAddr destination addr of elements to store read region
     * @para index the index start to read
     * @para nRead the num of elements need to be read. if nRead is 0, will try
     * to read everything can be read
     * @return the num of elements read success. return 0 if read failed
     */
    size_t getRegion(T* elementsAddr, size_t index, size_t nRead);

  private:
    size_t m_bufferSize;
    std::vector<T> m_buffer;
};

template <class T>
CircularBuffer<T>::CircularBuffer(size_t bufferSize)
    : m_bufferSize(bufferSize) {}

template <class T>
CircularBuffer<T>::~CircularBuffer() {}

template <typename T>
T& CircularBuffer<T>::operator[](size_t i) {
    return m_buffer[i];
}
template <class T>
void CircularBuffer<T>::push_back(T element) {
    m_buffer.push_back(element);
    if (m_buffer.size() > m_bufferSize) m_buffer.erase(begin());
}

template <class T>
void CircularBuffer<T>::push_front(T element) {
    m_buffer.insert(m_buffer.begin(), element);
    if (m_buffer.size() > m_bufferSize) m_buffer.pop_back();
}
template <class T>
const T& CircularBuffer<T>::back() const {
    return m_buffer.back();
}

template <class T>
const T& CircularBuffer<T>::front() const {
    return m_buffer.front();
}

template <class T>
typename std::vector<T>::iterator CircularBuffer<T>::begin() {
    return m_buffer.begin();
}

template <class T>
typename std::vector<T>::iterator CircularBuffer<T>::end() {
    return m_buffer.end();
}

template <class T>
typename std::vector<T>::reverse_iterator CircularBuffer<T>::rbegin() {
    return m_buffer.rbegin();
}

template <class T>
typename std::vector<T>::reverse_iterator CircularBuffer<T>::rend() {
    return m_buffer.rend();
}

template <class T>
typename std::vector<T>::const_iterator CircularBuffer<T>::begin() const {
    return m_buffer.begin();
}

template <class T>
typename std::vector<T>::const_iterator CircularBuffer<T>::end() const {
    return m_buffer.end();
}

template <class T>
typename std::vector<T>::const_reverse_iterator CircularBuffer<T>::rbegin()
    const {
    return m_buffer.rbegin();
}

template <class T>
typename std::vector<T>::const_reverse_iterator CircularBuffer<T>::rend()
    const {
    return m_buffer.rend();
}

template <class T>
bool CircularBuffer<T>::full() const {
    return m_buffer.size() == m_bufferSize;
}

template <class T>
bool CircularBuffer<T>::empty() const {
    return m_buffer.empty();
}

template <class T>
void CircularBuffer<T>::clear() {
    m_buffer.clear();
}

template <class T>
void CircularBuffer<T>::resize(size_t newSize) {
    if (newSize >= m_bufferSize)
        m_bufferSize = newSize;
    else {
        for (size_t i = 0; i < m_bufferSize - newSize; i++) {
            m_buffer.erase(begin());
        }
    }
}

template <class T>
typename std::vector<T>::size_type CircularBuffer<T>::size() const {
    return m_buffer.size();
}

template <class T>
size_t CircularBuffer<T>::pushRegion(const T* elementsAddr, size_t nWrite) {
    if (nWrite > m_bufferSize) {
        return 0;
    }
    size_t lastIndex = m_buffer.size() - 1;
    m_buffer.resize(nWrite + m_buffer.size());
    std::memcpy(&m_buffer[lastIndex + 1], elementsAddr, nWrite * sizeof(T));
    if ((lastIndex + nWrite) >= m_bufferSize) {
        // need to delete elements ahead buffer
        m_buffer.erase(
            m_buffer.begin(),
            m_buffer.begin() + ((lastIndex + nWrite) - m_bufferSize + 1));
    }
    return nWrite;
}

template <class T>
size_t CircularBuffer<T>::pushRegion(const T* elementsAddr,
                                     size_t nWrite,
                                     size_t& nDeleted) {
    if (nWrite > m_bufferSize) {
        return 0;
    }
    nDeleted = 0;
    size_t lastIndex = m_buffer.size() - 1;
    m_buffer.resize(nWrite + m_buffer.size());
    std::memcpy(&m_buffer[lastIndex + 1], elementsAddr, nWrite * sizeof(T));
    if ((lastIndex + nWrite) >= m_bufferSize) {
        // need to delete elements ahead buffer
        m_buffer.erase(
            m_buffer.begin(),
            m_buffer.begin() + ((lastIndex + nWrite) - m_bufferSize + 1));
        nDeleted = ((lastIndex + nWrite) - m_bufferSize + 1);
    }
    return nWrite;
}

template <class T>
size_t CircularBuffer<T>::getRegion(T* elementsAddr,
                                    size_t index,
                                    size_t nRead) {
    if (nRead > (m_bufferSize - index)) {
        return 0;
    }
    if (nRead == 0) {
        nRead = (m_buffer.size() - index);
    }
    std::memcpy(elementsAddr, &m_buffer[index], nRead * sizeof(T));
    return nRead;
}

}  // namespace DataStructures
}  // namespace Utils