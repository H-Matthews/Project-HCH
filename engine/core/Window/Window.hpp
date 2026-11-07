#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"
#include "core/Event.hpp"

#include <string>
#include <string_view>
#include <cstdint>
#include <functional>
#include <optional>

// TODO: REMOVE ASAP
#include <SFML/Window/Event.hpp>

namespace Core {

// TODO: Create Engine Event System
// Because the Engine does NOT yet have its own Event System,
// We are forced to have SFML in the WindowInterface
using EventCallbackFn = std::function<void(const sf::Event&)>;

class Window {
  public:
    static constexpr std::string_view SECTION_NAME = "Window";

    // Window Configurable Properties
    static constexpr std::string_view BACKEND = "backend";
    static constexpr std::string_view WIDTH = "width";
    static constexpr std::string_view HEIGHT = "height";
    static constexpr std::string_view TITLE = "title";
    static constexpr std::string_view VSYNC = "vsync";

    virtual ~Window() = default;

    virtual bool initialize(const ConfigSection& section) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual void shutdown() = 0;
    virtual std::optional<Event> pollEvents() = 0;
    virtual void swapBuffers() = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void* getNativeWindowHandle() const = 0;
};

} // namespace Core