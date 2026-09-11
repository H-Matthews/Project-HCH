#pragma once

#include <exception>
#include <string>

namespace Core {
class ConfigurationException : public std::exception {
  public:
    explicit ConfigurationException(std::string message) : mMessage(std::move(message)) {}

    ~ConfigurationException() = default;

    const char* what() const noexcept override { return mMessage.c_str(); }

  private:
    std::string mMessage;
};
} // namespace Core
