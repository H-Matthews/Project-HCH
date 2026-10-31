#include "application/Data/GameResources.hpp"

#include "core/Application.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>

void Application::loadGameResources(Core::Application& application) {
    const Core::DirectoryLayout& directories = application.getDirectories();

    application.getFonts().load(std::string(Fonts::ARIAL),
                                directories.assetFontsDirectory() + "/arial.ttf");
}
