#pragma once

#include "core/inc/ParserI.hpp"


namespace Core
{
    class IniParser : public ParserI
    {
        public:
            IniParser(const std::string& nameID, const std::string& fileExtension);

            bool parseFile(std::ifstream& fileStream) override;
    };
}