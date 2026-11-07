cmake_minimum_required(VERSION 3.30)

# Game layer (ECS content, states, key bindings, game messages), built on the engine
set(GAME_LIB_SRC
    Settings/KeyBindings.cpp
    Data/PlayerAction.cpp
    Data/GameResources.cpp
    Scene/Scene.cpp
    Scene/ECS/EntityFactory.cpp
    Scene/ECS/GameTuning.cpp
    Scene/ECS/Systems/CombatSystem.cpp
    Scene/ECS/Systems/EnemyAISystem.cpp
    Scene/ECS/Systems/HealthSystem.cpp
    Scene/ECS/Systems/MovementSystem.cpp
    Scene/ECS/Systems/PlayerInputSystem.cpp
    Scene/ECS/Systems/RenderSystem.cpp
    Scene/ECS/Systems/SpawnSystem.cpp
    StateStack/GameState.cpp
    StateStack/GameOverState.cpp
    StateStack/MenuState.cpp
    StateStack/PauseState.cpp)

set(GAME_LIB_INC
    Settings/KeyBindings.hpp
    Data/PlayerAction.hpp
    Data/GameResources.hpp
    Scene/Scene.hpp
    Scene/ECS/EntityFactory.hpp
    Scene/ECS/GameTuning.hpp
    Scene/ECS/Components/Components.hpp
    Scene/ECS/Components/GameplayComponents.hpp
    Scene/ECS/Components/PlayerInputComponent.hpp
    Scene/ECS/Systems/Systems.hpp
    Scene/ECS/Systems/CombatSystem.hpp
    Scene/ECS/Systems/EnemyAISystem.hpp
    Scene/ECS/Systems/HealthSystem.hpp
    Scene/ECS/Systems/MovementSystem.hpp
    Scene/ECS/Systems/PlayerInputSystem.hpp
    Scene/ECS/Systems/RenderSystem.hpp
    Scene/ECS/Systems/SpawnSystem.hpp
    UI/TextUtil.hpp
    StateStack/GameState.hpp
    StateStack/GameOverState.hpp
    StateStack/MenuState.hpp
    StateStack/PauseState.hpp)

list(TRANSFORM GAME_LIB_SRC PREPEND "src/application/")
list(TRANSFORM GAME_LIB_INC PREPEND "src/application/")

add_library(application STATIC ${GAME_LIB_SRC} ${GAME_LIB_INC})

if(MSVC)
  target_compile_options(application PRIVATE /W4)
else()
  target_compile_options(application PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(application PROPERTIES FOLDER HCH)

target_link_libraries(application PUBLIC engine SFML::Graphics SFML::Window SFML::System)
# Include root so game headers resolve as "application/..."
target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
# Root of this game's configs/, assets/ and output/ directories
target_compile_definitions(application PUBLIC GAME_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Executable
add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER HCH)
target_link_libraries(${PROJECT_NAME} PRIVATE application)
