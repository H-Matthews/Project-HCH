cmake_minimum_required(VERSION 3.30)
project(applicationLibrary LANGUAGES CXX)

# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(LIBRARY_NAME "application")

set(SRC src/App.cpp
        src/KeyBindings.cpp
        src/MessageNetwork/Publishers/PlayerInputPublisher.cpp
        src/MessageNetwork/Messages/PlayerInputMessage.cpp
        src/PlayerAction.cpp
        src/Scene/Scene.cpp
        src/Scene/ECS/Components/Sprite.cpp
        src/Scene/ECS/Components/PlayerInput.cpp
        src/Scene/ECS/Systems/BaseSystem.cpp
        src/Scene/ECS/Systems/RenderSystem.cpp
        src/Scene/ECS/Systems/PlayerMovementSystem.cpp
        src/Scene/ECS/Systems/PlayerInputSystem.cpp
        src/Scene/ECS/Entity.cpp
        src/Scene/ECS/EntityManager.cpp
        src/State/GameState.cpp
        src/State/MenuState.cpp
        src/State/PauseState.cpp)

set(INC inc/App.hpp
        inc/KeyBindings.hpp
        inc/MessageNetwork/Publishers/PlayerInputPublisher.hpp
        inc/MessageNetwork/Messages/PlayerInputMessage.hpp
        inc/PlayerAction.hpp
        inc/Scene/Scene.hpp
        inc/Scene/ECS/Components/Components.hpp
        inc/Scene/ECS/Components/Sprite.hpp
        inc/Scene/ECS/Components/PlayerInput.hpp
        inc/Scene/ECS/Systems/Systems.hpp
        inc/Scene/ECS/Systems/BaseSystem.hpp
        inc/Scene/ECS/Systems/RenderSystem.hpp
        inc/Scene/ECS/Systems/PlayerMovementSystem.hpp
        inc/Scene/ECS/Systems/PlayerInputSystem.hpp
        inc/Scene/ECS/Entity.hpp
        inc/Scene/ECS/EntityManager.hpp
        inc/State/GameState.hpp
        inc/State/MenuState.hpp
        inc/State/PauseState.hpp)

add_library(${LIBRARY_NAME} STATIC ${SRC} ${INC})

# Turns warnings ON
if(MSVC)
  target_compile_options(${LIBRARY_NAME} PRIVATE /W4 )
else()
  target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic )
endif()

# Allows Visual Studio to create a folder for the source
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System core utility)

target_compile_definitions(${LIBRARY_NAME} PRIVATE PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${LIBRARY_NAME} PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")