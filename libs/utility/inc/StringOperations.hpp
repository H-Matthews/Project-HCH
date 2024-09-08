
#pragma once

#include "State.hpp"
#include "Message.hpp"

#include <sstream>

namespace Utility
{
    const std::string statesEnumToString(States::ID identifier);

    const std::string messageEnumToString(Messages::ID identifier);

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