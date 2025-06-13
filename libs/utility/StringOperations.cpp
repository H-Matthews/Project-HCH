#include "utility/StringOperations.hpp"

#include <string>
#include <algorithm>
#include <iostream>

std::string Utility::ltrim( const std::string& stringToTrim, const std::string delimiter )
{
    size_t start = stringToTrim.find_first_not_of( delimiter );

    return ( start == std::string::npos ) ? "" : stringToTrim.substr( start );
}

std::string Utility::rtrim( const std::string& stringToTrim, const std::string delimiter )
{
    size_t end = stringToTrim.find_last_not_of( delimiter );

    return ( end == std::string::npos ) ? "" : stringToTrim.substr( 0, end + 1 );
}

std::string Utility::trimTrailingAndLeadingWhiteSpace( const std::string& stringToTrim )
{
    if ( containsWhiteSpace( stringToTrim ) )
        return rtrim( ltrim( stringToTrim ) );

    return stringToTrim;
}

bool Utility::containsWhiteSpace( const std::string& stringToCheck )
{
    // Determine if the string has whitespace... IF it does, then trim it
    size_t start = stringToCheck.find_first_not_of( Utility::WHITESPACE );
    if ( start == std::string::npos )
        return false;

    return true;
}

std::string Utility::removeAllSpaces( const std::string& stringToModify )
{
    std::string modifiedString( stringToModify );

    modifiedString.erase(
        std::remove_if( modifiedString.begin(), modifiedString.end(), isspace ), modifiedString.end() );

    return modifiedString;
}

std::string Utility::removeQuotes( const std::string& stringToModify )
{
    std::string modifiedString( stringToModify );

    modifiedString.erase( std::remove( modifiedString.begin(), modifiedString.end(), '\"' ), modifiedString.end() );

    return modifiedString;
}
