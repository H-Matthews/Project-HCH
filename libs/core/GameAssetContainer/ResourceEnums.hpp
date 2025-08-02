#pragma once

// Forward Declaration
namespace sf
{
    class Texture;
    class Font;
}

namespace Textures
{
    enum ID
    {
        PLAYER = 0,
        ENEMY,
        BACKGROUND
    };
}

namespace Fonts
{
    enum ID
    {
        ARIAL_FONT = 0,
        NASTY_FONT
    };
}

// Forward Declaration of ResourceHolder Class
template < typename Resource, typename Identifier >
class ResourceHolder;

typedef ResourceHolder< sf::Texture, Textures::ID > TextureHolder;
typedef ResourceHolder< sf::Font, Fonts::ID > FontHolder;