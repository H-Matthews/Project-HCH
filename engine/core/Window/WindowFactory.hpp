#pragma once

#include "core/Window/SfmlWindow.hpp"
#include "core/Configuration/ConfigSection/ConfigSection.hpp"
#include "core/Exceptions/ConfigurationException.hpp"

#include <memory>
#include <string>
#include <optional>

namespace Core {

enum class WindowBackend {
    SFML = 0,
    OpenGL_GLFW // NOT IMPLEMENTED
};

inline std::optional<WindowBackend> windowBackendFromString(
    const std::string& backendString) {

    std::optional<WindowBackend> windowBackendOpt;

    if(backendString == "SFML") {
        windowBackendOpt = WindowBackend::SFML;
    }
    else if(backendString == "OpenGL") {
        windowBackendOpt = WindowBackend::OpenGL_GLFW;
    }

    return windowBackendOpt;
}

namespace WindowFactory {

inline std::unique_ptr<Window> create(const ConfigSection& section) {
    std::unique_ptr<Window> window;

    // Defaults to SFML if property not found
    const auto backendType =
        section.getString(Window::BACKEND).value_or("SFML");

    std::optional<WindowBackend> windowBackendOpt =
        windowBackendFromString(backendType);
    if(!windowBackendOpt.has_value()) {
        throw ConfigurationException("Unknown window backend '" + backendType + "'");
    }

    switch(*windowBackendOpt) {
        case WindowBackend::SFML: {
            window = std::make_unique<SfmlWindow>();
            break;
        }
        case WindowBackend::OpenGL_GLFW: {
            // NOT IMPLEMENTED YET
            // window = std::make_unique<OpenGlWindow();
            throw ConfigurationException("Window backend '" + backendType + "' is not implemented");
        }
    }

    if(window && window->initialize(section) == false) {
        window.reset(nullptr);
    }

    return window;
}

} // NAMESPACE WINDOWFACTORY

} // NAMESPACE CORE