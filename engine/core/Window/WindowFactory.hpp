#pragma once

#include "core/Window/SfmlWindow.hpp"

#include <memory>

namespace Core {

enum class WindowBackend {
    SFML = 0,
    OpenGL_GLFW // NOT IMPLEMENTED
};

namespace WindowFactory {

static std::unique_ptr<Window> create(WindowBackend backendType, WindowSpec windowSpec) {
    std::unique_ptr<Window> window;

    switch(backendType) {
        case WindowBackend::SFML: {
            window = std::make_unique<SfmlWindow>();
            break;
        }
        case WindowBackend::OpenGL_GLFW: {
            // NOT IMPLEMENTED YET
            // window = std::make_unique<OpenGlWindow();
            break;
        }
    }

    if(window && window->initialize(std::move(windowSpec)) == false) {
        window.reset(nullptr);
    }

    return window;
}

} // NAMESPACE WINDOWFACTORY

} // NAMESPACE CORE