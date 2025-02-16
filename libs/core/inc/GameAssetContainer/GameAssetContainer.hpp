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

            std::unordered_map<std::string, sf::Texture> getTextureMap();
            std::unordered_map<std::string, sf::Font> getFontMap();

        private:
            std::unordered_map<std::string, sf::Texture> textureMap;
            std::unordered_map<std::string, sf::Font> fontMap;
    }


}