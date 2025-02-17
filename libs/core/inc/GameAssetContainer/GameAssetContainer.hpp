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

            sf::Texture* getTexture(std::string);
            std::unordered_map<std::string, sf::Font> getFontMap();
            ~GameAssetContainer();
        private:
            std::string filePath = "../../../gameAssets/textures/";
            bool loadTexture(std::string);
            std::unordered_map<std::string, sf::Texture*> textureMap;
            std::unordered_map<std::string, sf::Font> fontMap;

    };


}