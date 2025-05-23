#include "core/inc/Configuration/Parsers/ParserDataRegistry.hpp"

#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

std::shared_ptr< Core::ParserDataRegistry > Core::ParserDataRegistry::mRegistryInstance = nullptr;

Core::ParserDataRegistry::ParserDataRegistry() :
    mParserDataContainer(),
    mFullyQualifedDataName()
{}

std::shared_ptr< Core::ParserDataRegistry > Core::ParserDataRegistry::instance()
{
    if ( mRegistryInstance == nullptr )
    {
        mRegistryInstance = std::shared_ptr< Core::ParserDataRegistry >( new Core::ParserDataRegistry() );

        mRegistryInstance->initializeLogger();
    }

    return mRegistryInstance;
}

void Core::ParserDataRegistry::registerParserID( Parsers::ID ID )
{
    std::string logMessage;

    auto it = mParserDataContainer.find( ID );
    if ( it != mParserDataContainer.end() )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "Duplicate ParserID '" + Parsers::parserEnumsToString( ID ) + "' detected. " +
                         "second ID will be ignored";

            mLogger->logWarn( logMessage );

            return;
        }
    }

    if constexpr ( Utility::CAN_LOG )
    {
        logMessage = "Adding ParserID: '" + Parsers::parserEnumsToString( ID ) + "' to ParserDataRegistry";

        mLogger->logDebug( logMessage );
    }

    mParserDataContainer.insert( { ID, FileToDataMap{} } );

    return;
}

void Core::ParserDataRegistry::setParserData( Parsers::ID ID, const std::string& fileName, std::any parserData )
{
    std::string logMessage;

    auto parserIDIter = mParserDataContainer.find( ID );
    if ( parserIDIter == mParserDataContainer.end() )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "Unable to SetParserData because the ParserID '" + Parsers::parserEnumsToString( ID ) +
                         "' was not Registered";

            mLogger->logWarn( logMessage );
        }
        return;
    }

    auto result = parserIDIter->second.insert( { fileName, parserData } );
    if ( !result.second )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "Parser Data was already given for the file '" + fileName + "' and Parser ID '" +
                         Parsers::parserEnumsToString( ID ) +
                         "' the program is not currently configured to overwrite data";

            mLogger->logWarn( logMessage );
        }
        return;
    }

    if constexpr ( Utility::CAN_LOG )
    {
        logMessage =
            "Setting Parser Data for File '" + fileName + "' and ParserID '" + Parsers::parserEnumsToString( ID ) + "'";

        mLogger->logDebug( logMessage );
    }

    return;
}

std::any Core::ParserDataRegistry::getParserDataStructure( Parsers::ID ID, const std::string& fileName )
{
    std::any targetData;
    std::string logMessage;

    auto parserIDIter = mParserDataContainer.find( ID );
    if ( parserIDIter == mParserDataContainer.end() )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "Unable to locate ParserData with the ID '" + Parsers::parserEnumsToString( ID ) +
                         "' Unable to located Parser Data ";
            mLogger->logWarn( logMessage );
        }

        return targetData;
    }

    auto parserDataIter = parserIDIter->second.find( fileName );
    if ( parserDataIter == parserIDIter->second.end() )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "Unable to locate the file name '" + fileName + "' with Parser ID '" +
                         Parsers::parserEnumsToString( ID ) + "' Unable to locate Parser Data";
            mLogger->logWarn( logMessage );
        }

        return targetData;
    }

    targetData = parserDataIter->second;

    return targetData;
}

Core::FileToDataMap Core::ParserDataRegistry::getParserDataStructure( Parsers::ID ID )
{
    Core::FileToDataMap targetFileData;
    std::string logMessage;

    auto parserIDIter = mParserDataContainer.find( ID );
    if ( parserIDIter == mParserDataContainer.end() )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "Unable to locate ParserData with the ID '" + Parsers::parserEnumsToString( ID ) + "' ";
            mLogger->logWarn( logMessage );
        }

        return targetFileData;
    }

    targetFileData = parserIDIter->second;

    return targetFileData;
}

/**
 * This Logger should NOT be CONFIGURABLE
 */
void Core::ParserDataRegistry::initializeLogger()
{
    const std::string outDirectory = Utility::LogRegistry::instance()->getAppOutputDir();
    if ( outDirectory == "" )
        return;

    mLogger = Utility::createTextFileLogger(
        "ParserDataRegistry", outDirectory, "ParserDataRegistry", ".log", Utility::LogLevel::DEBUG );

    if constexpr ( Utility::CAN_LOG )
    {
        mLogger->logInfo( "Logger Initialized" );
        Utility::LogRegistry::instance()->getGlobalLogger()->logInfo( "Logger Initialized" );
    }

    return;
}