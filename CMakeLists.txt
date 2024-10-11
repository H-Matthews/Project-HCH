cmake_minimum_required(VERSION 3.16)
project(Project-HCH LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../application/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(SFML_INC_DIR ../../build/_deps/sfml-src/include)

add_subdirectory(libs/utility)
add_subdirectory(application)

set_property(TARGET hch PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/application/bin")