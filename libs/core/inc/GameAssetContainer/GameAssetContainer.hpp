#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

namespace Core
{
    enum class GameAssetType {
        PLAYER_TEXTURE = 1,
        ENEMY_TEXTURE = 2,
        ARIAL_FONT = 3,
        NASTY_FONT = 4
    };

    class GameAssetContainer{

        public:
            bool initializeTextures();
            bool initializeFonts();
            const sf::Texture* getTexture(GameAssetType textureName);
            const sf::Font* getFont(GameAssetType fontName);
            
        private:
            bool loadTexture(GameAssetType spriteName, const std::string &filePath);
            bool loadFont(GameAssetType fontName, const std::string &filePath);
            std::unordered_map<GameAssetType, std::unique_ptr<sf::Texture>> textureMap;
            std::unordered_map<GameAssetType, std::unique_ptr<sf::Font>> fontMap;

    };
}