cmake_minimum_required(VERSION 3.30)
project(utilLibrary LANGUAGES CXX)


set(LIBRARY_NAME "utility")

set(SRC Logging/LogRegistry.cpp
        Logging/Logger.cpp
        Logging/Sinks/ColorConsoleSink.cpp
        Logging/Sinks/TextFileSink.cpp
        Logging/Formatters/KeyValueFormatter.cpp
        Logging/Formatters/DefaultFormatter.cpp
        Logging/LogLevel.cpp
        StringOperations.cpp
        Time.cpp )

set(INC Logging/LogRegistry.hpp
        Logging/Logger.hpp
        Logging/Sinks/LogSink.hpp
        Logging/Sinks/ColorConsoleSink.hpp
        Logging/Sinks/TextFileSink.hpp
        Logging/Formatters/LogFormatter.hpp
        Logging/Formatters/KeyValueFormatter.hpp
        Logging/Formatters/DefaultFormatter.hpp
        Logging/LogLevel.hpp
        StringOperations.hpp
        Time.hpp )

add_library(${LIBRARY_NAME} STATIC ${SRC} ${INC})

# Turns warnings ON
if(MSVC)
  target_compile_options(${LIBRARY_NAME} PRIVATE /W4 )
else()
  target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic )
endif()

# Allows Visual Studio to create a folder for the source
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME} PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${LIBRARY_NAME} PRIVATE
    "$<$<CONFIG:DEBUG>:APP_DEBUG>"
    "$<$<BOOL:${LOG_LEVEL_DEFINE}>:${LOG_LEVEL_DEFINE}>")