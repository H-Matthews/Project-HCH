cmake_minimum_required(VERSION 3.30)
project(applicationLibrary LANGUAGES CXX)

# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(LIBRARY_NAME "application")

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