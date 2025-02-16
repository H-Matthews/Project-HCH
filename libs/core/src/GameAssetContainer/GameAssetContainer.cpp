#include "core/inc/GameAssetContainer/GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures()
{

    sf::Texture tempTexture;
    
    if(!tempTexture.loadFromFile("../../../gameAssets/textures/playerSprite.png")){
        //Add logging here for failed texture load.
        return false;
    }
    //Add logging here for completed texture load.
    textureMap.insert({"playerSprite" ,tempTexture});
    
    return true;
}

bool Core::GameAssetContainer::initializeFonts()
{

    sf::Font tempFont;

    //Creates undefined reference:
    /*
    if(!tempFont.openFromFile("../../../gameAssets/fonts/arial.tff")){
        //Add logging here for failed font load.
        return false;
    }
    */

    //Add logging here for completed font load.
    fontMap.insert({"arial" , tempFont});
    
    return true;
}

std::unordered_map<std::string, sf::Texture> Core::GameAssetContainer::getTextureMap()
{
    return textureMap;
}

std::unordered_map<std::string, sf::Font> Core::GameAssetContainer::getFontMap()
{
    return fontMap;
}
