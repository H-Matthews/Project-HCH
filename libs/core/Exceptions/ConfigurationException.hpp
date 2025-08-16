#pragma once

#include <exception>

namespace Core
{
    class ConfigurationException : public std::exception
    {
      public:
        ConfigurationException( const char* message ) :
            mMessage( message )
        {}

        ~ConfigurationException() = default;

        inline const char* what() const noexcept override;

      private:
        const char* mMessage;
    };

    const char* ConfigurationException::what() const noexcept
    {
        return mMessage;
    }
}