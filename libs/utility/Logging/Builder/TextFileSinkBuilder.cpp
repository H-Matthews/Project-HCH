#include "utility/Logging/Builder/TextFileSinkBuilder.hpp"

#include "utility/Logging/Sinks/TextFileSink.hpp"
#include "utility/Logging/LogLevel.hpp"

Utility::TextFileSinkBuilder::TextFileSinkBuilder(TextFileSink* sink) : mSink(sink) {}

Utility::TextFileSinkBuilder&
Utility::TextFileSinkBuilder::outputDirectory(const std::string& outputDirectory) {
    mSink->mOutputDirectory = outputDirectory;

    return *this;
}

Utility::TextFileSinkBuilder& Utility::TextFileSinkBuilder::fileName(const std::string& fileName) {
    mSink->mFileName = fileName;

    return *this;
}

Utility::TextFileSinkBuilder&
Utility::TextFileSinkBuilder::logExtension(const std::string& logExtension) {
    mSink->mLogExtension = logExtension;

    return *this;
}

Utility::TextFileSinkBuilder& Utility::TextFileSinkBuilder::logLevel(const std::string& logLevel) {
    Utility::LogLevel localLogLevel = stringToLogLevelEnum(logLevel);

    mSink->mLevel = localLogLevel;

    return *this;
}

Utility::TextFileSinkBuilder&
Utility::TextFileSinkBuilder::formatter(std::unique_ptr<LogFormatter> formatter) {
    mSink->mFormatter.swap(formatter);

    return *this;
}