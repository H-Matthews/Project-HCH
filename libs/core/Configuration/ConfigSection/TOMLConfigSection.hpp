#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include "vendor/toml/include/toml.hpp"

namespace Core
{
    /**
     * TOML-backed implementation of ConfigSection.
     * Wraps a toml::table and delegates all typed reads to toml++.
     * This is the only file in the Configuration module that includes toml++ headers;
     * all other code depends on the ConfigSection interface instead.
     */
    class TOMLConfigSection : public ConfigSection
    {
      public:
        explicit TOMLConfigSection( toml::table table );

        std::optional< bool > getBool( std::string_view key ) const override;
        std::optional< int > getInt( std::string_view key ) const override;
        std::optional< double > getDouble( std::string_view key ) const override;
        std::optional< std::string > getString( std::string_view key ) const override;
        std::vector< std::string > getStringVector( std::string_view key ) const override;

        std::unique_ptr< ConfigSection > getSection( std::string_view name ) const override;

      private:
        toml::table mTable;
    };

} // namespace Core
