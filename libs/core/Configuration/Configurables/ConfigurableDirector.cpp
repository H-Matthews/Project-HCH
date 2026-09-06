#include "core/Configuration/Configurables/ConfigurableDirector.hpp"

#include "core/Configuration/ConfigTree/ConfigUtils.hpp"

// Logger
#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"
#include "utility/Logging/Formatters/DefaultFormatter.hpp"

// TODO: This entire function needs work... Just did a quick job for proof of concept
std::shared_ptr<Utility::Logger>
Core::ConfigurableDirector::buildLogger(std::shared_ptr<ConfigNode> node) {
    std::shared_ptr<Utility::Logger> logger = nullptr;

    auto loggerNode = ConfigUtils::getConfigNode(node, "Logger");
    if (!loggerNode)
        return logger;

    // Determine if this configurable is creating a logger
    std::string loggerName;
    if (auto* name = loggerNode->get()->findValue<std::string>("logger_name"))
        loggerName = *name;

    std::string globalLogLevel;
    if (auto* level = loggerNode->get()->findValue<std::string>("global_log_level"))
        globalLogLevel = *level;

    // TODO: Instead of hard coding these sinks, we need to create an ENUM
    // of possible sinks, and cycle over that
    ConfigNode* sinkNode = nullptr;

    std::unique_ptr<Utility::LogFormatter> logFormatter = nullptr;
    std::vector<std::shared_ptr<Utility::LogSink>> sinks;

    sinkNode = loggerNode->get()->getChild("ColorConsoleSink");
    if (sinkNode) {
        // Determine if this configurable is creating a logger
        std::string log_level;
        if (auto* level = sinkNode->findValue<std::string>("log_level"))
            log_level = *level;

        std::string formatter;
        if (auto* format = sinkNode->findValue<std::string>("formatter"))
            formatter = *format;

        if (formatter == "DefaultFormatter") {
            logFormatter = std::make_unique<Utility::DefaultFormatter>();
        } else if (formatter == "KeyValueFormatter") {
            logFormatter = std::make_unique<Utility::KeyValueFormatter>();
        }

        Utility::ColorConsoleSink* sink =
            Utility::ColorConsoleSink::build().logLevel(log_level).formatter(
                std::move(logFormatter));

        sinks.push_back(std::shared_ptr<Utility::LogSink>(sink));
    }

    sinkNode = loggerNode->get()->getChild("TextFileSink");
    const std::string engineOutputDir = Utility::LogRegistry::instance()->getOutputDir();
    if (sinkNode) {
        std::string fileName;
        if (auto* name = sinkNode->findValue<std::string>("log_file_name"))
            fileName = *name;

        std::string fileExtension;
        if (auto* extension = sinkNode->findValue<std::string>("log_file_extension"))
            fileExtension = *extension;

        std::string logLevel;
        if (auto* level = sinkNode->findValue<std::string>("log_level"))
            logLevel = *level;

        std::string formatter;
        if (auto* format = sinkNode->findValue<std::string>("formatter"))
            formatter = *format;

        if (formatter == "DefaultFormatter") {
            logFormatter = std::make_unique<Utility::DefaultFormatter>();
        } else if (formatter == "KeyValueFormatter") {
            logFormatter = std::make_unique<Utility::KeyValueFormatter>();
        }

        Utility::TextFileSink* sink = Utility::TextFileSink::build()
                                          .outputDirectory(engineOutputDir)
                                          .fileName(fileName)
                                          .logExtension(fileExtension)
                                          .logLevel(logLevel)
                                          .formatter(std::move(logFormatter));

        sink->openFile();

        sinks.push_back(std::shared_ptr<Utility::LogSink>(sink));
    }

    Utility::Logger* rawLogger =
        Utility::Logger::build().name(loggerName).globalLogLevel(globalLogLevel).sinks(sinks);

    logger.reset(rawLogger);

    Utility::LogRegistry::instance()->registerLogger(logger);

    return logger;
}