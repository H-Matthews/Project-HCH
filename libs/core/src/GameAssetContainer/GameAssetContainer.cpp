#include "core/inc/GameAssetContainer/GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures()
{
    if(!loadTexture(GameAssetType::PLAYER_TEXTURE, "../../../gameAssets/textures/playerSprite.png")){
        return false;
    }
    if(!loadTexture(GameAssetType::ENEMY_TEXTURE, "../../../gameAssets/textures/enemySprite.png")){
        return false;
    }
    return true;
}

bool Core::GameAssetContainer::initializeFonts()
{
    if(!loadFont(GameAssetType::ARIAL_FONT, "../../../gameAssets/fonts/arial.ttf")){
        return false;
    }
    if(!loadFont(GameAssetType::NASTY_FONT, "../../../gameAssets/fonts/nasty.ttf")){
        return false;
    }
    return true;
}

const sf::Texture* Core::GameAssetContainer::getTexture(GameAssetType textureName)
{
    return textureMap[textureName].get();
}

const sf::Font* Core::GameAssetContainer::getFont(GameAssetType fontName)
{
    return fontMap[fontName].get();
}

bool Core::GameAssetContainer::loadTexture(GameAssetType textureName, const std::string &filePath)
{
    auto tempTexture(std::make_unique<sf::Texture>());

    if(!tempTexture->loadFromFile(filePath)){
        return false;
    }

    textureMap.insert({textureName, std::move(tempTexture)});

    return true;
}

bool Core::GameAssetContainer::loadFont(GameAssetType fontName, const std::string &filePath)
{
    auto tempFont(std::make_unique<sf::Font>());

    if(!tempFont->openFromFile(filePath)){
        return false;
    }

    fontMap.insert({fontName, std::move(tempFont)});
        
    return true;
}
