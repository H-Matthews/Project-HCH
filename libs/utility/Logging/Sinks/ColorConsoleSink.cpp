#include "utility/Logging/Sinks/ColorConsoleSink.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Formatters/DefaultFormatter.hpp"

#include <iostream>

#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

namespace {
bool detectTTY() {
#if defined(_WIN32)
    return _isatty(_fileno(stdout)) != 0;
#else
    return isatty(fileno(stdout)) != 0;
#endif
}
} // namespace

const std::string Utility::ColorConsoleSink::sinkIdentifier = "ColorConsoleSink";

Utility::ColorConsoleSink::ColorConsoleSink()
    : LogSink(sinkIdentifier), mOutputStream(std::cout), mUseColor(detectTTY()) {}

// Builder implementation

Utility::ColorConsoleSink::Builder::Builder() : mSink(std::make_shared<ColorConsoleSink>()) {}

Utility::ColorConsoleSink::Builder&
Utility::ColorConsoleSink::Builder::logLevel(const std::string& level) {
    mSink->setSinkLogLevel(stringToLogLevelEnum(level));
    return *this;
}

Utility::ColorConsoleSink::Builder&
Utility::ColorConsoleSink::Builder::formatter(std::unique_ptr<LogFormatter> f) {
    mSink->setFormatter(std::move(f));
    return *this;
}

std::shared_ptr<Utility::ColorConsoleSink> Utility::ColorConsoleSink::Builder::build() {
    return mSink;
}

Utility::ColorConsoleSink::Builder Utility::ColorConsoleSink::make() {
    return Builder{};
}

void Utility::ColorConsoleSink::sinkData(std::string_view message, LogLevel level,
                                         const std::source_location location) {
    if (mFormatter) {
        std::string formattedMessage = mFormatter->format(std::string(message), level, location);

        if (mUseColor) {
            const std::string colorCode = getColorCode(level);
            insertColorCodes(formattedMessage, colorCode);
        }

        mOutputStream << formattedMessage << '\n';
    }
}

void Utility::ColorConsoleSink::insertColorCodes(std::string& message,
                                                 const std::string& colorCode) const {
    message.insert(0, colorCode);
    message.insert(message.size(), mDefaultColorCode);
}

std::string Utility::ColorConsoleSink::getColorCode(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG:
        return mDebugColorCode;
    case LogLevel::INFO:
        return mInfoColorCode;
    case LogLevel::WARN:
        return mWarningColorCode;
    case LogLevel::ERROR:
        return mErrorColorCode;
    default:
        return mDefaultColorCode;
    }
}
