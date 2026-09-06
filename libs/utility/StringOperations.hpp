
#pragma once

#include <sstream>
#include <vector>

namespace Utility {
template <typename T> const std::string toString(const T& value);

template <typename T> const std::string toString(const T& value) {
    std::stringstream sStream;
    sStream << value;
    return sStream.str();
}

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& stringToTrim, const std::string delimiter = WHITESPACE);
std::string rtrim(const std::string& stringToTrim, const std::string delimiter = WHITESPACE);
std::string trimTrailingAndLeadingWhiteSpace(const std::string& stringToTrim);
bool containsWhiteSpace(const std::string& stringToCheck);

std::string removeAllSpaces(const std::string& stringToModify);
std::string removeQuotes(const std::string& stringToModify);

void splitString(const std::string& stringToSplit, std::vector<std::string>& splitStrings,
                 const char delimiter);

} // namespace Utility