#include <stdexcept>

template < typename Resource, typename Identifier >
void ResourceHolder< Resource, Identifier >::load( Identifier id, const std::string& fileName )
{
    auto resource = std::make_unique< Resource >();
    if ( !resource->loadFromFile( fileName ) )
        throw std::runtime_error( "ResourceHolder::load - Failed to load " + fileName );

    // If Successful...
    insertResource( id, std::move( resource ) );

    return;
}

template < typename Resource, typename Identifier >
Resource& ResourceHolder< Resource, Identifier >::get( Identifier id )
{
    auto found = mResourceMap.find( id );
    assert( found != mResourceMap.end() );

    return *found->second;
}

template < typename Resource, typename Identifier >
const Resource& ResourceHolder< Resource, Identifier >::get( Identifier id ) const
{
    auto found = mResourceMap.find( id );
    assert( found != mResourceMap.end() );

    return *found->second;
}

template < typename Resource, typename Identifier >
void ResourceHolder< Resource, Identifier >::insertResource( Identifier id, std::unique_ptr< Resource > resource )
{
    // Insert and check success
    auto inserted = mResourceMap.insert( std::make_pair( id, std::move( resource ) ) );
    assert( inserted.second );

    return;
}