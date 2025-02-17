#include "core/inc/GameAssetContainer/GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures()
{
    //TODO Issues:
    //All of variables pointers should be shared pointers if possible.
    //I think the main problem here is that when this function goes out of scope, the tempTexture
    //variable goes out of scope causing the pointer variable to be lost.
    sf::Texture tempTexture;
    
    if(!tempTexture.loadFromFile("../../../gameAssets/textures/playerSprite.png")){
        //TODO Add logging here for failed texture load.
        return false;
    }
    //Add logging here for completed texture load.
    textureMap.insert({"playerSprite" ,&tempTexture});
    
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
