cmake_minimum_required(VERSION 3.30)

# The engine: game-agnostic foundation (core + utility + vendored headers) as one static library.
# Include paths are rooted at this directory, e.g. #include "core/Application.hpp",
# "utility/Logging/Logger.hpp", "vendor/entt/include/entt.hpp".
set(ENGINE_SRC
    core/Application.cpp
    core/StateStack/State.cpp
    core/StateStack/StateStack.cpp
    core/Window/SfmlWindow.cpp
    core/Window/SfmlEventTranslate.cpp
    core/Configuration/Configuration.cpp
    core/Configuration/DirectoryLayout.cpp
    core/Configuration/ConfigReader/TOMLConfigReader.cpp
    core/Configuration/ConfigSection/TOMLConfigSection.cpp
    core/Configuration/LoggerBuilder.cpp
    core/ecs/BaseSystem.cpp
    core/ecs/Entity.cpp
    core/ecs/EntityManager.cpp
    utility/Logging/LogRegistry.cpp
    utility/Logging/Logger.cpp
    utility/Logging/Sinks/ColorConsoleSink.cpp
    utility/Logging/Sinks/TextFileSink.cpp
    utility/Logging/Formatters/KeyValueFormatter.cpp
    utility/Logging/Formatters/DefaultFormatter.cpp
    utility/Logging/LogLevel.cpp
    utility/StringOperations.cpp
    utility/Time.cpp)

set(ENGINE_INC
    core/Application.hpp
    core/StateStack/State.hpp
    core/StateStack/StateStack.hpp
    core/Window/Window.hpp
    core/Window/SfmlEventTranslate.hpp
    core/Window/SfmlWindow.hpp
    core/Window/WindowFactory.hpp
    core/Event.hpp
    core/InputState.hpp
    core/Exceptions/ConfigurationException.hpp
    core/Configuration/Configuration.hpp
    core/Configuration/DirectoryLayout.hpp
    core/Configuration/ConfigReader/ConfigReader.hpp
    core/Configuration/ConfigReader/TOMLConfigReader.hpp
    core/Configuration/ConfigSection/ConfigSection.hpp
    core/Configuration/ConfigSection/TOMLConfigSection.hpp
    core/Configuration/LoggerBuilder.hpp
    core/GameAssetContainer/ResourceHolder.hpp
    core/GameAssetContainer/ResourceTypes.hpp
    core/ecs/BaseSystem.hpp
    core/ecs/Entity.hpp
    core/ecs/EntityManager.hpp
    utility/Logging/LogRegistry.hpp
    utility/Logging/Logger.hpp
    utility/Logging/Sinks/LogSink.hpp
    utility/Logging/Sinks/ColorConsoleSink.hpp
    utility/Logging/Sinks/TextFileSink.hpp
    utility/Logging/Formatters/LogFormatter.hpp
    utility/Logging/Formatters/KeyValueFormatter.hpp
    utility/Logging/Formatters/DefaultFormatter.hpp
    utility/Logging/LogLevel.hpp
    utility/StringOperations.hpp
    utility/Time.hpp
    utility/VectorMath.hpp)

add_library(engine STATIC ${ENGINE_SRC} ${ENGINE_INC})

if(MSVC)
  target_compile_options(engine PRIVATE /W4)
else()
  target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(engine PROPERTIES FOLDER engine)

target_link_libraries(engine PUBLIC SFML::Graphics)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(engine PUBLIC cxx_std_20)
target_compile_definitions(engine PUBLIC
    "$<$<CONFIG:DEBUG>:APP_DEBUG>"
    "$<$<BOOL:${LOG_LEVEL_DEFINE}>:${LOG_LEVEL_DEFINE}>")
