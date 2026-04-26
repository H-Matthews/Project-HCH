cmake_minimum_required(VERSION 3.30)
project(applicationLibrary LANGUAGES CXX)

# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(LIBRARY_NAME "application")

set(SRC Settings/KeyBindings.cpp
        Messaging/Publishers/PlayerInputPublisher.cpp
        Messaging/Messages/PlayerInputMessage.cpp
        Data/PlayerAction.cpp
        Scene/Scene.cpp
        Scene/ECS/Components/SpriteComponent.cpp
        Scene/ECS/Components/PlayerInputComponent.cpp
        Scene/ECS/Systems/BaseSystem.cpp
        Scene/ECS/Systems/RenderSystem.cpp
        Scene/ECS/Systems/PlayerMovementSystem.cpp
        Scene/ECS/Systems/PlayerInputSystem.cpp
        Scene/ECS/Entity.cpp
        Scene/ECS/EntityManager.cpp
        StateStack/GameState.cpp
        StateStack/MenuState.cpp
        StateStack/PauseState.cpp)

set(INC Settings/KeyBindings.hpp
        Messaging/Publishers/PlayerInputPublisher.hpp
        Messaging/Messages/PlayerInputMessage.hpp
        Data/PlayerAction.hpp
        Scene/Scene.hpp
        Scene/ECS/Components/Components.hpp
        Scene/ECS/Components/SpriteComponent.hpp
        Scene/ECS/Components/PlayerInputComponent.hpp
        Scene/ECS/Systems/Systems.hpp
        Scene/ECS/Systems/BaseSystem.hpp
        Scene/ECS/Systems/RenderSystem.hpp
        Scene/ECS/Systems/PlayerMovementSystem.hpp
        Scene/ECS/Systems/PlayerInputSystem.hpp
        Scene/ECS/Entity.hpp
        Scene/ECS/EntityManager.hpp
        StateStack/GameState.hpp
        StateStack/MenuState.hpp
        StateStack/PauseState.hpp)

add_library(${LIBRARY_NAME} STATIC ${SRC} ${INC})

# Turns warnings ON
if(MSVC)
  target_compile_options(${LIBRARY_NAME} PRIVATE /W4 )
else()
  target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic )
endif()

# Allows Visual Studio to create a folder for the source
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME} PUBLIC sfml-graphics sfml-window sfml-system core utility)

target_compile_definitions(${LIBRARY_NAME} PUBLIC PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${LIBRARY_NAME} PRIVATE
    "$<$<CONFIG:DEBUG>:APP_DEBUG>"
    "$<$<BOOL:${LOG_LEVEL_DEFINE}>:${LOG_LEVEL_DEFINE}>")