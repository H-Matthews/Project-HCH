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
std::shared_ptr<sf::Texture> Core::GameAssetContainer::getTexture(std::string selectionString)
{
    return textureMap[selectionString];
}

std::shared_ptr<sf::Font> Core::GameAssetContainer::getFont(std::string fontName)
{
    return fontMap[fontName];
}

bool Core::GameAssetContainer::loadTexture(std::string spriteName, std::string filePath)
{
    std::shared_ptr<sf::Texture> tempTexture(std::make_shared<sf::Texture>());

    if(!tempTexture->loadFromFile(filePath)){
        return false;
    }

    textureMap.insert({spriteName ,tempTexture});

    return true;
}

bool Core::GameAssetContainer::loadFont(std::string fontName, std::string filePath)
{
    std::shared_ptr<sf::Font> tempFont(std::make_shared<sf::Font>());

    if(!tempFont->openFromFile(filePath)){
        return false;
    }

    fontMap.insert({fontName ,tempFont});
        
    return true;
}
