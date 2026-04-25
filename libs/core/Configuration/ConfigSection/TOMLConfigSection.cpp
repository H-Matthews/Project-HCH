#include "core/Configuration/ConfigSection/TOMLConfigSection.hpp"

Core::TOMLConfigSection::TOMLConfigSection( toml::table table ) :
    mTable( std::move( table ) )
{}

std::optional< bool > Core::TOMLConfigSection::getBool( std::string_view key ) const
{
    return mTable[ key ].value< bool >();
}

std::optional< int > Core::TOMLConfigSection::getInt( std::string_view key ) const
{
    auto val = mTable[ key ].value< int64_t >();
    if (!val.has_value())
        return std::nullopt;
    return static_cast< int >( val.value() );
}

std::optional< double > Core::TOMLConfigSection::getDouble( std::string_view key ) const
{
    return mTable[ key ].value< double >();
}

std::optional< std::string > Core::TOMLConfigSection::getString( std::string_view key ) const
{
    return mTable[ key ].value< std::string >();
}

std::vector< std::string > Core::TOMLConfigSection::getStringVector( std::string_view key ) const
{
    std::vector< std::string > result;
    const auto* arr = mTable[ key ].as_array();
    if (!arr)
        return result;

    for (const auto& elem : *arr)
    {
        if (auto s = elem.value< std::string >())
            result.push_back( std::move( *s ) );
    }

    return result;
}

std::unique_ptr< Core::ConfigSection > Core::TOMLConfigSection::getSection( std::string_view name ) const
{
    const auto* nested = mTable[ name ].as_table();
    if (!nested)
        return nullptr;
    return std::make_unique< TOMLConfigSection >( *nested );
}
