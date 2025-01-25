cmake_minimum_required(VERSION 3.30)
project(applicationLibrary LANGUAGES CXX)

# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(SRC src/App.cpp
        src/KeyBindings.cpp
        src/State/GameState.cpp
        src/State/MenuState.cpp
        src/State/PauseState.cpp
        src/Messages/PlayerInputMessage.cpp)

set(INC inc/App.hpp
        inc/KeyBindings.hpp
        inc/State/GameState.hpp
        inc/State/MenuState.hpp
        inc/State/PauseState.hpp
        inc/Messages/PlayerInputMessage.hpp)

add_library(application STATIC ${SRC} ${INC})

# Allows Visual Studio to create a folder for the source
set_target_properties(application PROPERTIES FOLDER "application")

target_link_libraries(application PRIVATE sfml-graphics sfml-window sfml-system core utility)

target_compile_definitions(application PRIVATE PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(application PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(application PRIVATE cxx_std_20)
target_compile_definitions(application PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")