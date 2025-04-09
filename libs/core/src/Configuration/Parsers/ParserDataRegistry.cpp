#include "core/inc/Configuration/Parsers/ParserDataRegistry.hpp"

std::shared_ptr< Core::ParserDataRegistry > Core::ParserDataRegistry::mRegistryInstance = nullptr;

Core::ParserDataRegistry::ParserDataRegistry() :
    mParserData()
{}

std::shared_ptr< Core::ParserDataRegistry > Core::ParserDataRegistry::instance()
{
    if (mRegistryInstance == nullptr)
        mRegistryInstance = std::shared_ptr< Core::ParserDataRegistry >( new Core::ParserDataRegistry() );

    return mRegistryInstance;
}

void Core::ParserDataRegistry::registerParserID( Parsers::ID ID )
{
    auto it = mParserData.find( ID );
    if (it != mParserData.end())
    {
        // Log
        return;
    }

    mParserData.insert( { ID, std::any{} } );

    return;
}

void Core::ParserDataRegistry::setParserData( Parsers::ID ID, std::any parserData )
{
    auto it = mParserData.find( ID );
    if (it == mParserData.end())
    {
        // Log and Return
        return;
    }

    it->second = parserData;

    return;
}

std::any Core::ParserDataRegistry::getParserData( Parsers::ID ID )
{
    std::any targetData;

    auto it = mParserData.find( ID );
    if (it != mParserData.end())
    {
        targetData = it->second;
    }

    return targetData;
}