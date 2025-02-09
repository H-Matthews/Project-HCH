#pragma once

#include <fstream>
#include <string>

namespace Core
{

    class ParserI
    {
        public:
            inline ParserI(const std::string nameIdentifier, const std::string fileExtension) :
                mNameIdentifier(nameIdentifier),
                mFileExtension(fileExtension) {}

            virtual bool parseFile(std::ifstream& fileStream) = 0;

            virtual ~ParserI() {}

        public:
            std::string mNameIdentifier;
            std::string mFileExtension;
    };
}