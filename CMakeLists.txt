cmake_minimum_required(VERSION 3.30)
project(Project-HCH LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

enable_language(C CXX)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppresses some annoying warnings
cmake_policy(SET CMP0135 NEW)

# Enable CTest
enable_testing()

# Enable Visual Studio to create folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Compile-time minimum log level. Empty = derive from build config (Debug=>DEBUG, Release=>WARN).
# Override with -DLOG_LEVEL=<NONE|DEBUG|INFO|WARN|ERROR>.
set(LOG_LEVEL "" CACHE STRING
    "Compile-time minimum log level: NONE, DEBUG, INFO, WARN, ERROR. Empty derives from build config.")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS "" NONE DEBUG INFO WARN ERROR)

set(LOG_LEVEL_DEFINE "")
if(LOG_LEVEL)
    set(_VALID_LOG_LEVELS NONE DEBUG INFO WARN ERROR)
    if(NOT LOG_LEVEL IN_LIST _VALID_LOG_LEVELS)
        message(FATAL_ERROR "LOG_LEVEL must be one of: ${_VALID_LOG_LEVELS} (got '${LOG_LEVEL}')")
    endif()
    set(LOG_LEVEL_DEFINE "LOG_LEVEL_${LOG_LEVEL}")
endif()

# Determine build type depending on generator used
if(CMAKE_CONFIGURATION_TYPES)
    set(CONFIG_TYPE "")
else() # Single Generator
    set(CONFIG_TYPE ${CMAKE_BUILD_TYPE})
endif()

# This changes depending on Single VS Multi generator
set(BUILD_DIRECTORY "${PROJECT_SOURCE_DIR}/build/${CONFIG_TYPE}")

# Dependency Include Directories
set(SFML_INC_DIR ${BUILD_DIRECTORY}/_deps/sfml-src/include)
set(GTEST_INC_DIR ${BUILD_DIRECTORY}/_deps/googletest-src/googletest/include)

# Include Directories
set(LIBS_INC_DIR ${PROJECT_SOURCE_DIR}/libs)

add_subdirectory(tests)
add_subdirectory(libs/core)
add_subdirectory(libs/utility)
add_subdirectory(libs/application)
add_subdirectory(application)