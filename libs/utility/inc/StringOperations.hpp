
#pragma once

#include "State.hpp"
#include "Message.hpp"

#include <sstream>

namespace Utility
{
    const std::string statesEnumToString(const States::ID identifier);

    const std::string messageEnumToString(const Message::ID identifier);

    template <typename T>
    const std::string& toString(const T& value);
}

template <typename T>
const std::string& toString(const T& value)
{
    std::stringstream sStream;
    sStream << value;
    return sStream.str();
}