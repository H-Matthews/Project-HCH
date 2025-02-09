#pragma once

#include "core/inc/Configuration/Parsers/Parser.hpp"


namespace Core
{
    class JsonParser : public Parser
    {
        public:
            JsonParser(const std::string parserIdentifierString);

            void parseFile(std::ifstream& fileStream) override;
    };
}