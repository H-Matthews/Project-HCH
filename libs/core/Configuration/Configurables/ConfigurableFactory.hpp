#pragma once

#include <map>
#include <string>
#include <functional>
#include <memory>

namespace Core
{
    class Configurable;

    /**
     * Factory uses a registration approach for creating objects, which will
     * move the responsbility of knowing which class to create out of this class
     */
    class ConfigurableFactory
    {
      public:
        using FactoryFunction = std::function< std::shared_ptr< Core::Configurable >() >;

        static void registerType( const std::string& typeName, FactoryFunction factoryFunction )
        {
            mRegistry.insert( std::make_pair( typeName, factoryFunction ) );
        }

        template < typename T >
        static std::shared_ptr< T > createTypedConfigurable( const std::string& typeName )
        {
            auto found = mRegistry.find( typeName );
            if ( found != mRegistry.end() )
            {
                std::shared_ptr< Configurable > baseConfigurable = found->second();
                std::shared_ptr< T > typedConfigurable = nullptr;

                typedConfigurable = std::dynamic_pointer_cast< T >( baseConfigurable );
                if ( typedConfigurable )
                    return typedConfigurable;
            }

            return nullptr;
        }

      private:
        inline static std::map< std::string, FactoryFunction > mRegistry;
    };
}