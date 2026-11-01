#include "core/Window/SfmlWindow.hpp"

#include <SFML/Window/VideoMode.hpp>

namespace Core {

bool SfmlWindow::initialize(WindowSpec windowSpec) {
    sf::Vector2u windowSize{windowSpec.width,windowSpec.height};

    mSfmlWindow.create(
        sf::VideoMode(windowSize), windowSpec.windowName, sf::Style::Close);

    return mSfmlWindow.isOpen();
}

void SfmlWindow::close() {
    if(mSfmlWindow.isOpen()) {
        mSfmlWindow.close();
    }
}

// Don't need a shutdown for SFML. When the sf::RenderWindow
// goes out of scope, its resources are cleaned up (RAII)
void SfmlWindow::shutdown() {
}

void SfmlWindow::pollEvents() {
    while(const std::optional<sf::Event> sfEvent = mSfmlWindow.pollEvent()) {
        if(mEventCallback) {
            mEventCallback(*sfEvent);
        }
    }
}

void SfmlWindow::swapBuffers() {
    // SFML handles buffer swapping automatically
    mSfmlWindow.display();
}

void* SfmlWindow::getNativeWindowHandle() const {
    return reinterpret_cast<void*>(mSfmlWindow.getNativeHandle());
}

void SfmlWindow::setEventCallback(EventCallbackFn callback) {
    mEventCallback = std::move(callback);
}

} // NAMESPACE CORE