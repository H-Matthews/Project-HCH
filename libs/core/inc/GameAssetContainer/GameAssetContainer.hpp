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
            sf::Texture tempTexture;

            sf::Texture* getTexture(std::string);
            std::unordered_map<std::string, sf::Font> getFontMap();

        private:
            std::unordered_map<std::string, sf::Texture*> textureMap;
            std::unordered_map<std::string, sf::Font> fontMap;
    };


}