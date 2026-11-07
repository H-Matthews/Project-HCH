#pragma once

#include "core/GameAssetContainer/ResourceTypes.hpp"

namespace sf {
class RenderWindow;
}

namespace Core {

class Window;
class InputState;

struct Context {
    Window& window;

    // Null if the window backend is not SFML.
    // TEMPORARY: required on draw() calls until the engine has its own render abstraction.
    sf::RenderWindow* renderWindow;

    const InputState& input;

    FontHolder& fonts;
    TextureHolder& textures;
};

} // NAMESPACE CORE
