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
    if(!loadFont("arial")){
        return false;
    }
    return true;
}

//TODO make getTexture and getFont faster by avoiding string compare
std::shared_ptr<sf::Texture> Core::GameAssetContainer::getTexture(std::string selectionString)
{
    return textureMap[selectionString];
}

sf::Font* Core::GameAssetContainer::getFont(std::string fontName)
{
    return fontMap[fontName];
}

bool Core::GameAssetContainer::loadTexture(std::string spriteName)
{
    std::shared_ptr<sf::Texture> tempTexture(new sf::Texture);

    std::string texturePath = filePath + "textures/" + spriteName + ".png";

    if(!tempTexture->loadFromFile(texturePath)){
        //TODO Add logging here for failed texture load./
        return false;
    }
    //Add logging here for completed texture load.
    textureMap.insert({spriteName ,tempTexture});
    return true;

}

bool Core::GameAssetContainer::loadFont(std::string fontName)
{
    
    sf::Font *tempFont = new sf::Font;

    std::string fontPath = filePath + "fonts/" + fontName + ".ttf";

    if(!tempFont->openFromFile(fontPath)){
        //TODO Add logging here for failed texture load.
        return false;
    }
    //Add logging here for completed texture load.
    fontMap.insert({fontName ,tempFont});
        
    return true;
}

Core::GameAssetContainer::~GameAssetContainer()
{

    //delete all fontPointers
    for(auto const& valuePair : fontMap){
        delete valuePair.second;
    }
}
