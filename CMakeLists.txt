cmake_minimum_required(VERSION 3.30)
project(Project-HCH LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppresses some annoying warnings
cmake_policy(SET CMP0135 NEW)

# Enable CTest
enable_testing()

################
# GTEST 
################
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/d144031940543e15423a25ae5a8a74141044862f.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


################
# SFML 
################
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Include Directories 
set(LIBS_INC_DIR ${PROJECT_SOURCE_DIR}/libs)

set(SFML_INC_DIR ${PROJECT_SOURCE_DIR}/build/_deps/sfml-src/include)
set(GTEST_INC_DIR ${PROJECT_SOURCE_DIR}/build/_deps/googletest-src/googletest/include)

add_subdirectory(tests)
add_subdirectory(libs/core)
add_subdirectory(libs/utility)
add_subdirectory(application)

set_property(TARGET hch PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/app")