#pragma once

#include "core/inc/Configuration/Parsers/ParserData.hpp"

#include <map>
#include <string>

namespace Core
{

    typedef std::map< std::string, std::pair< std::string, std::string > > IniData;

    class IniParserData : public ParserData
    {
      public:
        void setData( std::any data ) override;
        std::any getData() override;

      private:
        IniData mIniData;
    };
}