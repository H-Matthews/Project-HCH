#include "core/inc/GameAssetContainer/GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures()
{
    if(!loadTexture("playerSprite", "../../../gameAssets/textures/playerSprite.png")){
        return false;
    }
    if(!loadTexture("enemySprite", "../../../gameAssets/textures/enemySprite.png")){
        return false;
    }
    return true;
}

bool Core::GameAssetContainer::initializeFonts()
{
    if(!loadFont("arial", "../../../gameAssets/fonts/arial.ttf")){
        return false;
    }
    if(!loadFont("nasty", "../../../gameAssets/fonts/nasty.ttf")){
        return false;
    }
    return true;
}

//TODO maybe make string selection and ENUM for consistency
const sf::Texture* Core::GameAssetContainer::getTexture(const std::string &textureName)
{
    return textureMap[textureName].get();
}

const sf::Font* Core::GameAssetContainer::getFont(const std::string &fontName)
{
    return fontMap[fontName].get();
}

bool Core::GameAssetContainer::loadTexture(const std::string &spriteName, const std::string &filePath)
{
    auto tempTexture(std::make_unique<sf::Texture>());

    if(!tempTexture->loadFromFile(filePath)){
        return false;
    }

    textureMap.insert({spriteName, std::move(tempTexture)});

    return true;
}

bool Core::GameAssetContainer::loadFont(const std::string &fontName, const std::string &filePath)
{
    auto tempFont(std::make_unique<sf::Font>());

    if(!tempFont->openFromFile(filePath)){
        return false;
    }

    fontMap.insert({fontName, std::move(tempFont)});
        
    return true;
}
