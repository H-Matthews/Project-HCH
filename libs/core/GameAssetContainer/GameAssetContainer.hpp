#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

namespace Core {
enum class GameAssetType {
    ERROR_TEXTURE = 1,
    ERROR_FONT = 2,
    PLAYER_TEXTURE = 3,
    ENEMY_TEXTURE = 4,
    ARIAL_FONT = 5,
    NASTY_FONT = 6
};

class GameAssetContainer {

  public:
    bool initializeTextures();
    bool initializeFonts();
    const sf::Texture& getTexture(GameAssetType textureName);
    const sf::Font& getFont(GameAssetType fontName);

  private:
    bool loadTexture(GameAssetType spriteName, const std::string& filePath);
    bool loadFont(GameAssetType fontName, const std::string& filePath);
    std::unordered_map<GameAssetType, std::unique_ptr<sf::Texture>> textureMap;
    std::unordered_map<GameAssetType, std::unique_ptr<sf::Font>> fontMap;
};
} // namespace Core