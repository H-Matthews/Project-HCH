cmake_minimum_required(VERSION 3.30)
project(coreLibrary LANGUAGES CXX)


# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(LIBRARY_NAME "core")

set(SRC Messaging/Message.cpp
        Messaging/MessageNetwork.cpp
        Messaging/MessageNode.cpp
        Messaging/MessageTypes.cpp
        StateStack/State.cpp 
        StateStack/StateStack.cpp
        StateStack/StateTypes.cpp
        Configuration/Configuration.cpp
        Configuration/Parsers/ParserDataRegistry.cpp
        Configuration/Parsers/ParserTypes.cpp
        Configuration/Parsers/IniParser.cpp
        GameAssetContainer/GameAssetContainer.cpp )

set(INC Messaging/Message.hpp
        Messaging/MessageNetwork.hpp
        Messaging/MessageNode.hpp
        StateStack/State.hpp 
        StateStack/StateStack.hpp
        Configuration/ConfigurationI.hpp
        Configuration/Configuration.hpp
        Configuration/Parsers/ParserDataRegistry.hpp
        Configuration/Parsers/Parser.hpp
        Configuration/Parsers/ParserTypes.hpp
        Configuration/Parsers/IniParser.hpp
        GameAssetContainer/GameAssetContainer.hpp
        GameAssetContainer/ResourceHolder.hpp
        GameAssetContainer/ResourceEnums.hpp )

add_library(${LIBRARY_NAME} STATIC ${SRC} ${INC})

# Turns warnings ON
if(MSVC)
  target_compile_options(${LIBRARY_NAME} PRIVATE /W4 )
else()
  target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic )
endif()

# Allows Visual Studio to create a folder for the source
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME} PRIVATE SFML::Graphics)

target_compile_definitions(${LIBRARY_NAME} PRIVATE PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${LIBRARY_NAME} PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")