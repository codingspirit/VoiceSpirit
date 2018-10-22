#pragma once
#include <string>

namespace BaseClass {
class BaseException : public std::exception {
  public:
    explicit BaseException(
        const std::string& msg)  // implicit conversions are not allowed
        : m_msg{msg} {};
    virtual const char* what() const throw() { return m_msg.c_str(); };

  private:
    std::string m_msg;
};

}  // namespace BaseClass
