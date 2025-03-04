#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

namespace Core
{
    class GameAssetContainer{

        public:
            bool initializeTextures();
            bool initializeFonts();
            std::shared_ptr<sf::Texture> getTexture(std::string);
            std::shared_ptr<sf::Font> getFont(std::string);
            
        private:
            std::string filePath = "../../../gameAssets/";
            bool loadTexture(std::string);
            bool loadFont(std::string);
            std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureMap;
            std::unordered_map<std::string, std::shared_ptr<sf::Font>> fontMap;

    };
}