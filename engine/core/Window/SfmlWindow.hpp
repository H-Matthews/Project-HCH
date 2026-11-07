#pragma once

#include "core/Window/Window.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Core {

class SfmlWindow : public Window {
  public:
    // From Window
    bool initialize(const ConfigSection& section) override;
    void close() override;
    bool isOpen() const override;
    void shutdown() override;
    std::optional<Event> pollEvents() override;
    void swapBuffers() override;

    uint32_t getWidth() const override { return mSfmlWindow.getSize().x; }
    uint32_t getHeight() const override { return mSfmlWindow.getSize().y; }

    void* getNativeWindowHandle() const override;

    // SFML combines BOTH the OS window and the 2D
    // graphics renderer into a single object. So, these must be here
    void clear(const sf::Color& color) { mSfmlWindow.clear(color); }
    void setView(const sf::View& view) { mSfmlWindow.setView(view); }
    sf::RenderWindow& getRenderWindow() { return mSfmlWindow; }
    sf::View getDefaultView() const { return mSfmlWindow.getDefaultView(); }

  private:
    sf::RenderWindow mSfmlWindow;
};

} // namespace Core