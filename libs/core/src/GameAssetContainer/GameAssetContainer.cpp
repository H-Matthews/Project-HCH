#include "core/inc/GameAssetContainer/GameAssetContainer.hpp"


bool Core::GameAssetContainer::initializeTextures()
{
    //TODO add logging
    if(!loadTexture("playerSprite")){
        return false;
    }
    if(!loadTexture("enemySprite")){
        return false;
    }
    return true;
}

bool Core::GameAssetContainer::initializeFonts()
{
/*
    sf::Font tempFont;

    //TODO Bug Creates undefined reference:
    
    if(!tempFont.openFromFile("../../../gameAssets/fonts/arial.tff")){
        //TODO dd logging here for failed font load.
        return false;
    }
    
    //TODO Add logging here for completed font load.
    fontMap.insert({"arial" , tempFont});
    */

    return true;
}

sf::Texture* Core::GameAssetContainer::getTexture(std::string selectionString)
{
    return textureMap[selectionString];
}

std::unordered_map<std::string, sf::Font> Core::GameAssetContainer::getFontMap()
{
    return fontMap;
}

bool Core::GameAssetContainer::loadTexture(std::string spriteName)
{
    sf::Texture *tempTexture = new sf::Texture;

    std::string texturePath = filePath + spriteName + ".png";

    if(!tempTexture->loadFromFile(texturePath)){
        //TODO Add logging here for failed texture load.
        return false;
    }
    //Add logging here for completed texture load.
    textureMap.insert({spriteName ,tempTexture});
    return true;

}

Core::GameAssetContainer::~GameAssetContainer()
{
    for(auto const& valuePair : textureMap){
        delete valuePair.second;
    }
}
