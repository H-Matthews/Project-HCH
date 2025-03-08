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
            const sf::Texture* getTexture(const std::string &textureName);
            const sf::Font* getFont(const std::string &fontName);
            
        private:
            bool loadTexture(const std::string &spriteName, const std::string &filePath);
            bool loadFont(const std::string &fontName, const std::string &filePath);
            std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureMap;
            std::unordered_map<std::string, std::unique_ptr<sf::Font>> fontMap;

    };
}