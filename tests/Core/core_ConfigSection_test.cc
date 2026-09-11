#include <gtest/gtest.h>

#include "core/Configuration/ConfigSection/TOMLConfigSection.hpp"
#include "core/Configuration/LoggerBuilder.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"

#include "vendor/toml/include/toml.hpp"

#include <filesystem>

// -----------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------

static Core::TOMLConfigSection makeSection(std::string_view tomlContent) {
    return Core::TOMLConfigSection(toml::parse(tomlContent));
}

// -----------------------------------------------------------------------
// TOMLConfigSection — scalar getters
// -----------------------------------------------------------------------

class CoreConfigSectionScalarTest : public ::testing::Test {
  protected:
    Core::TOMLConfigSection section = makeSection(R"(
        flag   = true
        count  = 42
        ratio  = 2.5
        label  = "hello"
    )");
};

TEST_F(CoreConfigSectionScalarTest, getBool_ReturnsValue) {
    ASSERT_TRUE(section.getBool("flag").has_value());
    EXPECT_TRUE(section.getBool("flag").value());
}

TEST_F(CoreConfigSectionScalarTest, getBool_MissingKey_ReturnsNullopt) {
    EXPECT_FALSE(section.getBool("missing").has_value());
}

TEST_F(CoreConfigSectionScalarTest, getInt_ReturnsValue) {
    ASSERT_TRUE(section.getInt("count").has_value());
    EXPECT_EQ(section.getInt("count").value(), 42);
}

TEST_F(CoreConfigSectionScalarTest, getInt_MissingKey_ReturnsNullopt) {
    EXPECT_FALSE(section.getInt("missing").has_value());
}

TEST_F(CoreConfigSectionScalarTest, getDouble_ReturnsValue) {
    ASSERT_TRUE(section.getDouble("ratio").has_value());
    EXPECT_DOUBLE_EQ(section.getDouble("ratio").value(), 2.5);
}

TEST_F(CoreConfigSectionScalarTest, getDouble_MissingKey_ReturnsNullopt) {
    EXPECT_FALSE(section.getDouble("missing").has_value());
}

TEST_F(CoreConfigSectionScalarTest, getString_ReturnsValue) {
    ASSERT_TRUE(section.getString("label").has_value());
    EXPECT_EQ(section.getString("label").value(), "hello");
}

TEST_F(CoreConfigSectionScalarTest, getString_MissingKey_ReturnsNullopt) {
    EXPECT_FALSE(section.getString("missing").has_value());
}

// -----------------------------------------------------------------------
// TOMLConfigSection — string vector
// -----------------------------------------------------------------------

class CoreConfigSectionVectorTest : public ::testing::Test {
  protected:
    Core::TOMLConfigSection section = makeSection(R"(
        sinks   = ["ColorConsoleSink", "TextFileSink"]
        empty   = []
    )");
};

TEST_F(CoreConfigSectionVectorTest, getStringVector_ReturnsAllElements) {
    auto vec = section.getStringVector("sinks");
    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], "ColorConsoleSink");
    EXPECT_EQ(vec[1], "TextFileSink");
}

TEST_F(CoreConfigSectionVectorTest, getStringVector_EmptyArray_ReturnsEmptyVector) {
    EXPECT_TRUE(section.getStringVector("empty").empty());
}

TEST_F(CoreConfigSectionVectorTest, getStringVector_MissingKey_ReturnsEmptyVector) {
    EXPECT_TRUE(section.getStringVector("missing").empty());
}

// -----------------------------------------------------------------------
// TOMLConfigSection — nested sections
// -----------------------------------------------------------------------

class CoreConfigSectionNestedTest : public ::testing::Test {
  protected:
    Core::TOMLConfigSection section = makeSection(R"(
        [Logger]
        logger_name = "TestLogger"

        [Logger.ColorConsoleSink]
        log_level = "INFO"
    )");
};

TEST_F(CoreConfigSectionNestedTest, getSection_ReturnsNonNull) {
    auto logger = section.getSection("Logger");
    EXPECT_NE(logger, nullptr);
}

TEST_F(CoreConfigSectionNestedTest, getSection_MissingSection_ReturnsNull) {
    EXPECT_EQ(section.getSection("Missing"), nullptr);
}

TEST_F(CoreConfigSectionNestedTest, getSection_ReadsValuesFromSubsection) {
    auto logger = section.getSection("Logger");
    ASSERT_NE(logger, nullptr);
    ASSERT_TRUE(logger->getString("logger_name").has_value());
    EXPECT_EQ(logger->getString("logger_name").value(), "TestLogger");
}

TEST_F(CoreConfigSectionNestedTest, getSection_ChainedNavigation) {
    auto logger = section.getSection("Logger");
    ASSERT_NE(logger, nullptr);

    auto sink = logger->getSection("ColorConsoleSink");
    ASSERT_NE(sink, nullptr);
    EXPECT_EQ(sink->getString("log_level").value_or(""), "INFO");
}

// -----------------------------------------------------------------------
// buildLogger
// -----------------------------------------------------------------------

class CoreBuildLoggerTest : public ::testing::Test {
  protected:
    void SetUp() override {
        auto tempDir = std::filesystem::temp_directory_path() / "hch_test_output";
        std::filesystem::create_directories(tempDir);
        Utility::LogRegistry::instance()->configureRegistry(tempDir.string());
    }
};

TEST_F(CoreBuildLoggerTest, loggingDisabled_ReturnsNull) {
    auto section = makeSection(R"( logging_enabled = false )");
    EXPECT_EQ(Core::buildLogger(section), nullptr);
}

TEST_F(CoreBuildLoggerTest, loggingEnabled_NoLoggerSection_ReturnsNull) {
    auto section = makeSection(R"( logging_enabled = true )");
    EXPECT_EQ(Core::buildLogger(section), nullptr);
}

TEST_F(CoreBuildLoggerTest, loggingEnabled_NoSinks_ReturnsEmptyLogger) {
    auto section = makeSection(R"(
        logging_enabled = true

        [Logger]
        logger_name     = "test_no_sinks"
        global_log_level = "NONE"
        sinks           = []
    )");

    auto logger = Core::buildLogger(section);
    ASSERT_NE(logger, nullptr);
    EXPECT_EQ(logger->getLoggerName(), "test_no_sinks");
    EXPECT_EQ(logger->getSinkReferences().size(), 0u);
}

TEST_F(CoreBuildLoggerTest, loggingEnabled_ColorConsoleSink_ReturnsLoggerWithOneSink) {
    auto section = makeSection(R"(
        logging_enabled = true

        [Logger]
        logger_name      = "test_console_sink"
        global_log_level = "NONE"
        sinks            = ["ColorConsoleSink"]

        [Logger.ColorConsoleSink]
        log_level = "INFO"
        formatter = "DefaultFormatter"
    )");

    auto logger = Core::buildLogger(section);
    ASSERT_NE(logger, nullptr);
    EXPECT_EQ(logger->getLoggerName(), "test_console_sink");
    ASSERT_EQ(logger->getSinkReferences().size(), 1u);

    auto* sink = dynamic_cast<Utility::ColorConsoleSink*>(logger->getSinkReferences()[0]);
    EXPECT_NE(sink, nullptr);
}

TEST_F(CoreBuildLoggerTest, loggingEnabled_TextFileSink_ReturnsLoggerWithOneSink) {
    auto section = makeSection(R"(
        logging_enabled = true

        [Logger]
        logger_name      = "test_file_sink"
        global_log_level = "NONE"
        sinks            = ["TextFileSink"]

        [Logger.TextFileSink]
        log_file_name      = "test_log"
        log_file_extension = ".log"
        log_level          = "INFO"
        formatter          = "KeyValueFormatter"
    )");

    auto logger = Core::buildLogger(section);
    ASSERT_NE(logger, nullptr);
    EXPECT_EQ(logger->getLoggerName(), "test_file_sink");
    ASSERT_EQ(logger->getSinkReferences().size(), 1u);

    auto* sink = dynamic_cast<Utility::TextFileSink*>(logger->getSinkReferences()[0]);
    EXPECT_NE(sink, nullptr);
}

TEST_F(CoreBuildLoggerTest, buildLogger_RegistersWithLogRegistry) {
    auto section = makeSection(R"(
        logging_enabled = true

        [Logger]
        logger_name      = "test_registry_lookup"
        global_log_level = "NONE"
        sinks            = []
    )");

    Core::buildLogger(section);

    auto retrieved = Utility::LogRegistry::instance()->getLogger("test_registry_lookup");
    EXPECT_NE(retrieved, nullptr);
}
