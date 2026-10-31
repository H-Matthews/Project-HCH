#pragma once

#include <string>

// Forward Declaration
namespace sf {
class Texture;
class Font;
} // namespace sf

// Forward Declaration of ResourceHolder Class
template <typename Resource, typename Identifier> class ResourceHolder;

// Resources are identified by string keys. Core does not know which resources a game has; the
// game layer defines its keys and loads the files.
typedef ResourceHolder<sf::Texture, std::string> TextureHolder;
typedef ResourceHolder<sf::Font, std::string> FontHolder;
