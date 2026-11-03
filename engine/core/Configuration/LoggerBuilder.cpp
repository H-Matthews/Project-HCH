#include "core/Configuration/LoggerBuilder.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/DefaultFormatter.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

static std::unique_ptr<Utility::LogFormatter> makeFormatter(const std::string& name) {
    if (name == "KeyValueFormatter") {
        return std::make_unique<Utility::KeyValueFormatter>();
    }
    return std::make_unique<Utility::DefaultFormatter>();
}

std::shared_ptr<Utility::Logger> Core::buildLogger(const Core::ConfigSection& config) {
    using namespace Utility;

    if (!config.getBool(Logger::LOGGING_ENABLED).value_or(false)) {
        return nullptr;
    }

    auto loggerSection = config.getSection(Logger::SECTION_NAME);
    if (!loggerSection) {
        return nullptr;
    }

    const auto loggerName = loggerSection->getString(Logger::LOGGER_NAME).value_or("");
    const auto globalLogLevel = loggerSection->getString(Logger::GLOBAL_LOG_LEVEL).value_or("NONE");
    const auto sinkNames = loggerSection->getStringVector(Logger::SINKS);

    std::vector<std::shared_ptr<LogSink>> sinks;

    for (const auto& sinkName : sinkNames) {
        auto sinkSection = loggerSection->getSection(sinkName);
        if (!sinkSection) {
            continue;
        }

        const auto logLevel = sinkSection->getString(Logger::LOG_LEVEL).value_or("INFO");
        const auto formatterName = sinkSection->getString(Logger::FORMATTER)
                                       .value_or(std::string(DefaultFormatter::IDENTIFIER));

        if (sinkName == ColorConsoleSink::IDENTIFIER) {
            auto sink = ColorConsoleSink::make()
                            .logLevel(logLevel)
                            .formatter(makeFormatter(formatterName))
                            .build();
            sinks.push_back(sink);
        } else if (sinkName == TextFileSink::IDENTIFIER) {
            const auto fileName = sinkSection->getString(Logger::LOG_FILE_NAME).value_or("log");
            const auto fileExtension =
                sinkSection->getString(Logger::LOG_FILE_EXTENSION).value_or(".log");

            auto sink = Utility::TextFileSink::make()
                            .outputDirectory(LogRegistry::instance()->getOutputDir())
                            .fileName(fileName)
                            .logExtension(fileExtension)
                            .logLevel(logLevel)
                            .formatter(makeFormatter(formatterName))
                            .build();
            sinks.push_back(sink);
        }
    }

    auto logger = Logger::make()
                      .name(loggerName)
                      .globalLogLevel(stringToLogLevelEnum(globalLogLevel))
                      .sinks(sinks)
                      .build();

    LogRegistry::instance()->registerLogger(logger);
    return logger;
}
