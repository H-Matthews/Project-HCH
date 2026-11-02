#include "core/Window/SfmlWindow.hpp"

#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include <SFML/Window/VideoMode.hpp>

namespace Core {

bool SfmlWindow::initialize(const ConfigSection& section) {
    const auto width =
        static_cast<uint32_t>(section.getInt(Window::WIDTH).value_or(1280));
    const auto height =
        static_cast<uint32_t>(section.getInt(Window::HEGIHT).value_or(720));
    const auto title =
        section.getString(Window::TITLE).value_or("default");

    sf::Vector2u windowSize{width, height};

    mSfmlWindow.create(
        sf::VideoMode(windowSize), title, sf::Style::Close);

    return mSfmlWindow.isOpen();
}

void SfmlWindow::close() {
    if(mSfmlWindow.isOpen()) {
        mSfmlWindow.close();
    }
}

bool SfmlWindow::isOpen() const {
    return mSfmlWindow.isOpen();
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