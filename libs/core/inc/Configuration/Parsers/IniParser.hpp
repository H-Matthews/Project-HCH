#pragma once

#include "core/inc/Configuration/Parsers/Parser.hpp"


namespace Core
{
    class IniParser : public Parser
    {
        public:
            IniParser(const std::string parserIdentifierString);

            void parseFile(std::ifstream& fileStream) override;
    };
}