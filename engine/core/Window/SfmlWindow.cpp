#include "core/Window/SfmlWindow.hpp"

#include "core/Window/SfmlEventTranslate.hpp"
#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include <SFML/Window/VideoMode.hpp>

namespace Core {

bool SfmlWindow::initialize(const ConfigSection& section) {
    constexpr int DEFAULT_WIDTH = 1280;
    constexpr int DEFAULT_HEIGHT = 720;

    // Non-positive sizes would wrap around when cast to unsigned
    const int rawWidth = section.getInt(Window::WIDTH).value_or(DEFAULT_WIDTH);
    const int rawHeight = section.getInt(Window::HEIGHT).value_or(DEFAULT_HEIGHT);
    const auto width = static_cast<uint32_t>(rawWidth > 0 ? rawWidth : DEFAULT_WIDTH);
    const auto height = static_cast<uint32_t>(rawHeight > 0 ? rawHeight : DEFAULT_HEIGHT);
    const auto title = section.getString(Window::TITLE).value_or("default");
    const bool vsync = section.getBool(Window::VSYNC).value_or(true);

    sf::Vector2u windowSize{width, height};

    mSfmlWindow.create(sf::VideoMode(windowSize), title, sf::Style::Close);
    mSfmlWindow.setVerticalSyncEnabled(vsync);

    // This should always be off
    mSfmlWindow.setKeyRepeatEnabled(false);

    return mSfmlWindow.isOpen();
}

void SfmlWindow::close() {
    if (mSfmlWindow.isOpen()) {
        mSfmlWindow.close();
    }
}

bool SfmlWindow::isOpen() const {
    return mSfmlWindow.isOpen();
}

// Don't need a shutdown for SFML. When the sf::RenderWindow
// goes out of scope, its resources are cleaned up (RAII)
void SfmlWindow::shutdown() {}

std::optional<Event> SfmlWindow::pollEvents() {
    while(const auto sfEvent = mSfmlWindow.pollEvent()) {
        if(auto engineEvent = translate(*sfEvent)) {
            return engineEvent;
        }
    }

    return std::nullopt;
}

void SfmlWindow::swapBuffers() {
    // SFML handles buffer swapping automatically
    mSfmlWindow.display();
}

void* SfmlWindow::getNativeWindowHandle() const {
    return reinterpret_cast<void*>(mSfmlWindow.getNativeHandle());
}

} // namespace Core