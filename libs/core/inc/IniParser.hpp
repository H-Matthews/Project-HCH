#pragma once

#include "core/inc/Parser.hpp"


namespace Core
{
    class IniParser : public Parser
    {
        public:
            IniParser(const std::string parserIdentifierString);

            void parseFile(std::ifstream& fileStream) override;
    };
}