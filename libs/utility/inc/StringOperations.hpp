
#pragma once

#include "core/inc/State/State.hpp"
#include "core/inc/Messaging/Message.hpp"

#include <sstream>

namespace Utility
{
    const std::string statesEnumToString(const States::ID identifier);

    const std::string messageIDEnumToString(const Messages::ID identifier);

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