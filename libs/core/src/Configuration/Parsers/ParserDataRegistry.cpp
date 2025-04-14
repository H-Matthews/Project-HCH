#include "core/inc/Configuration/Parsers/ParserDataRegistry.hpp"

std::shared_ptr< Core::ParserDataRegistry > Core::ParserDataRegistry::mRegistryInstance = nullptr;

Core::ParserDataRegistry::ParserDataRegistry() :
    mParserDataContainer(),
    mFullyQualifedDataName()
{}

std::shared_ptr< Core::ParserDataRegistry > Core::ParserDataRegistry::instance()
{
    if ( mRegistryInstance == nullptr )
        mRegistryInstance = std::shared_ptr< Core::ParserDataRegistry >( new Core::ParserDataRegistry() );

    return mRegistryInstance;
}

void Core::ParserDataRegistry::registerParserID( Parsers::ID ID )
{
    auto it = mParserDataContainer.find( ID );
    if ( it != mParserDataContainer.end() )
    {
        // Log
        return;
    }

    mParserDataContainer.insert( { ID, FileToDataMap{} } );

    return;
}

void Core::ParserDataRegistry::setParserData( Parsers::ID ID, const std::string& fileName, std::any parserData )
{
    auto parserIDIter = mParserDataContainer.find( ID );
    if ( parserIDIter == mParserDataContainer.end() )
    {
        // Log
        return;
    }

    auto result = parserIDIter->second.insert( { fileName, parserData } );
    if ( !result.second )
    {
        // Log Key was already present NOT inserting
    }

    return;
}

std::any Core::ParserDataRegistry::getParserDataStructure( Parsers::ID ID, const std::string& fileName )
{
    std::any targetData;

    auto parserIDIter = mParserDataContainer.find( ID );
    if ( parserIDIter != mParserDataContainer.end() )
    {
        auto parserDataIter = parserIDIter->second.find( fileName );
        if ( parserDataIter != parserIDIter->second.end() )
            targetData = parserDataIter->second;
    }

    return targetData;
}