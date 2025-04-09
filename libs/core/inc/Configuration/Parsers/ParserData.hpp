#pragma once

#include <any>

namespace Core
{
    /**
     * Interface Class
     */

    class ParserData
    {
      public:
        virtual void setData( std::any data ) = 0;
        virtual std::any getData() = 0;
    };
}