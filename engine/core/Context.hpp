#pragma once

namespace Core {

class Window;
class InputState;
class Application;

struct Context {
    Window& window;
    const InputState& input;

    // This is just so we can get the sf::RenderWindow
    // Should NOT be used for anything else. The sf::RenderWindow is required
    // on draw() calls for now.
    Application& application;
};

} // NAMESPACE CORE