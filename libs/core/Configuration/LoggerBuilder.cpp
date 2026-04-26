#include "core/Configuration/LoggerBuilder.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/DefaultFormatter.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

static std::unique_ptr< Utility::LogFormatter > makeFormatter( const std::string& name )
{
    if (name == "KeyValueFormatter")
        return std::make_unique< Utility::KeyValueFormatter >();
    return std::make_unique< Utility::DefaultFormatter >();
}

std::shared_ptr< Utility::Logger > Core::buildLogger( const Core::ConfigSection& config )
{
    if (!config.getBool( "logging_enabled" ).value_or( false ))
        return nullptr;

    auto loggerSection = config.getSection( "Logger" );
    if (!loggerSection)
        return nullptr;

    const auto loggerName = loggerSection->getString( "logger_name" ).value_or( "" );
    const auto globalLogLevel = loggerSection->getString( "global_log_level" ).value_or( "NONE" );
    const auto sinkNames = loggerSection->getStringVector( "sinks" );

    std::vector< std::shared_ptr< Utility::LogSink > > sinks;

    for (const auto& sinkName : sinkNames)
    {
        auto sinkSection = loggerSection->getSection( sinkName );
        if (!sinkSection)
            continue;

        const auto logLevel = sinkSection->getString( "log_level" ).value_or( "INFO" );
        const auto formatterName = sinkSection->getString( "formatter" ).value_or( "DefaultFormatter" );

        if (sinkName == "ColorConsoleSink")
        {
            auto sink = Utility::ColorConsoleSink::make()
                            .logLevel( logLevel )
                            .formatter( makeFormatter( formatterName ) )
                            .build();
            sinks.push_back( sink );
        }
        else if (sinkName == "TextFileSink")
        {
            const auto fileName = sinkSection->getString( "log_file_name" ).value_or( "log" );
            const auto fileExtension = sinkSection->getString( "log_file_extension" ).value_or( ".log" );

            auto sink = Utility::TextFileSink::make()
                            .outputDirectory( Utility::LogRegistry::instance()->getOutputDir() )
                            .fileName( fileName )
                            .logExtension( fileExtension )
                            .logLevel( logLevel )
                            .formatter( makeFormatter( formatterName ) )
                            .build();
            sinks.push_back( sink );
        }
    }

    auto logger = Utility::Logger::make()
                      .name( loggerName )
                      .globalLogLevel( Utility::stringToLogLevelEnum( globalLogLevel ) )
                      .sinks( sinks )
                      .build();

    Utility::LogRegistry::instance()->registerLogger( logger );
    return logger;
}
