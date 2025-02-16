#include "GameAssetContainer.hpp"

bool Core::GameAssetContainer::initializeTextures()
{

    sf::Texture tempTexture;
    
    if(!tempTexture.loadFromFile("../../gameAssets/textures/playerSprite.png")){
        //Add logging here.
        return false;
    }
    textureMap.insert({"playerSprite" ,tempTexture});
    
    return true;
}

bool Core::GameAssetContainer::initializeFonts()
{
    return false;
}

std::unordered_map<std::string, sf::Texture> Core::GameAssetContainer::getTextureMap()
{
    return textureMap;
}

std::unordered_map<std::string, sf::Font> Core::GameAssetContainer::getFontMap()
{
    return fontMap;
}
