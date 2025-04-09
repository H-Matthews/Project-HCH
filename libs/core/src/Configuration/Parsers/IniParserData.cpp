#include "core/inc/Configuration/Parsers/IniParserData.hpp"

void Core::IniParserData::setData( std::any genericData )
{
    // Ensure the data uses the correct data structure for the data type (INI)
    IniData* iniData = std::any_cast< IniData >( &genericData );
    if ( !iniData )
    {
        // Log and Return
        return;
    }

    mIniData = *( iniData );

    return;
}

std::any Core::IniParserData::getData()
{
    return std::any( mIniData );
}