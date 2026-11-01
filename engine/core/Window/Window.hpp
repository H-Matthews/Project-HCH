#pragma once

#include <string>
#include <cstdint>
#include <functional>

// TODO: REMOVE ASAP
#include <SFML/Window/Event.hpp>

namespace Core {

// TODO: Create Engine Event System
// Because the Engine does NOT yet have its own Event System,
// We are forced to have SFML in the WindowInterface
using EventCallbackFn = std::function<void(const sf::Event&)>;

struct WindowSpec {
    std::string windowName = "Default Window";
    uint32_t width = 1280;
    uint32_t height = 720;
    bool vSync = true;

    WindowSpec() = default;
    WindowSpec(const WindowSpec&) = delete;
    WindowSpec& operator=(const WindowSpec&) = delete;
    WindowSpec(WindowSpec&&) = default;
    WindowSpec& operator=(WindowSpec&&) = default;
};

class Window {
public:
    virtual ~Window() = default;

    virtual bool initialize(WindowSpec windowSpec) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual void shutdown() = 0;
    virtual void pollEvents() = 0;
    virtual void swapBuffers() = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void* getNativeWindowHandle() const = 0;
    virtual void setEventCallback(EventCallbackFn callback) = 0;
};

} // NAMESPACE CORE