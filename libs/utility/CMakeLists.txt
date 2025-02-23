cmake_minimum_required(VERSION 3.30)
project(utilLibrary LANGUAGES CXX)


set(LIBRARY_NAME "utility")

set(SRC src/Logging/LogRegistry.cpp
        src/Logging/Logger.cpp
        src/Logging/Sinks/ColorConsoleSink.cpp
        src/Logging/Sinks/TextFileSink.cpp
        src/Logging/Formatters/KeyValueFormatter.cpp
        src/Logging/Formatters/DefaultFormatter.cpp
        src/Logging/LogLevel.cpp )

set(INC inc/Logging/LogRegistry.hpp
        inc/Logging/Logger.hpp
        inc/Logging/Sinks/LogSinksI.hpp
        inc/Logging/Sinks/ColorConsoleSink.hpp
        inc/Logging/Sinks/TextFileSink.hpp
        inc/Logging/Formatters/FormatterI.hpp
        inc/Logging/Formatters/KeyValueFormatter.hpp
        inc/Logging/Formatters/DefaultFormatter.hpp
        inc/Logging/LogLevel.hpp
        inc/StringOperations.hpp )

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
target_compile_definitions(${LIBRARY_NAME} PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")