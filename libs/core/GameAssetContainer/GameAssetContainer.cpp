#include "core/GameAssetContainer/GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures() {
    // TODO probably remove this later on and add this logic to be baked into the sprites
    // themselves. That way they are built into the binary.
    if (!loadTexture(GameAssetType::ERROR_TEXTURE,
                     "../../../gameAssets/textures/errorSprite.png")) {
        return false;
    }
    if (!loadTexture(GameAssetType::PLAYER_TEXTURE,
                     "../../../gameAssets/textures/playerSprite.png")) {
        return false;
    }
    if (!loadTexture(GameAssetType::ENEMY_TEXTURE,
                     "../../../gameAssets/textures/enemySprite.png")) {
        return false;
    }
    return true;
}

bool Core::GameAssetContainer::initializeFonts() {
    // TODO probably remove this later on and add this logic to be baked into the font themselves.
    // That way they are built into the binary. Not sure how possible this is but I know you can do
    // it with sprites.
    if (!loadFont(GameAssetType::ERROR_FONT, "../../../gameAssets/fonts/error.ttf")) {
        return false;
    }
    if (!loadFont(GameAssetType::ARIAL_FONT, "../../../gameAssets/fonts/arial.ttf")) {
        return false;
    }
    if (!loadFont(GameAssetType::NASTY_FONT, "../../../gameAssets/fonts/nasty.ttf")) {
        return false;
    }
    return true;
}

const sf::Texture& Core::GameAssetContainer::getTexture(GameAssetType textureName) {
    auto it = textureMap.find(textureName);
    if (it != textureMap.end()) {
        return *it->second.get();
    }
    // ERROR print and then exit program.
    throw std::filesystem::filesystem_error("Texture " + std::to_string(int(textureName)) +
                                                " was referenced, but not loaded.",
                                            std::error_code());
}

const sf::Font& Core::GameAssetContainer::getFont(GameAssetType fontName) {

    auto it = fontMap.find(fontName);
    if (it != fontMap.end()) {
        return *it->second.get();
    }
    // ERROR print and then exit program.
    throw std::filesystem::filesystem_error("Font " + std::to_string(int(fontName)) +
                                                " was referenced, but not loaded.",
                                            std::error_code());
}

bool Core::GameAssetContainer::loadTexture(GameAssetType textureName, const std::string& filePath) {
    auto tempTexture(std::make_unique<sf::Texture>());

    if (!tempTexture->loadFromFile(filePath)) {
        return false;
    }

    textureMap.insert({textureName, std::move(tempTexture)});

    return true;
}

bool Core::GameAssetContainer::loadFont(GameAssetType fontName, const std::string& filePath) {
    auto tempFont(std::make_unique<sf::Font>());

    if (!tempFont->openFromFile(filePath)) {
        return false;
    }

    fontMap.insert({fontName, std::move(tempFont)});

    return true;
}
