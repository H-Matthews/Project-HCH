#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"
#include "core/Configuration/ConfigSection/TOMLConfigSection.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

std::unique_ptr<Core::ConfigSection> Core::TOMLConfigReader::readFile(const std::filesystem::path& filePath)
{
    if (!std::filesystem::is_regular_file(filePath))
        return nullptr;

    try
    {
        toml::table table = toml::parse_file(filePath.string());
        return std::make_unique<TOMLConfigSection>(std::move(table));
    }
    catch (const toml::parse_error& e)
    {
        throw ConfigurationException( "TOML parse error in: " + filePath.string() + " - " + e.what() );
    }
}
