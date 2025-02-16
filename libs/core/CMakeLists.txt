cmake_minimum_required(VERSION 3.30)
project(coreLibrary LANGUAGES CXX)


# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(LIBRARY_NAME "core")

set(SRC src/Messaging/Message.cpp
        src/Messaging/MessageNetwork.cpp
        src/Messaging/MessageNode.cpp
        src/Messaging/MessageNodeInfo.cpp
        src/Messaging/MessageTypes.cpp
        src/State/State.cpp 
        src/State/StateStack.cpp
        src/State/StateTypes.cpp
        src/SceneNode.cpp
        src/Configuration/Configuration.cpp
        src/Configuration/Parsers/ParserTypes.cpp
        src/Configuration/Parsers/IniParser.cpp
        src/Configuration/Parsers/JsonParser.cpp
        src/GameAssetContainer/GameAssetContainer.cpp )

set(INC inc/Messaging/Message.hpp
        inc/Messaging/MessageNetwork.hpp
        inc/Messaging/MessageNode.hpp
        inc/Messaging/MessageNodeInfo.hpp
        inc/State/State.hpp 
        inc/State/StateStack.hpp
        inc/SceneNode.hpp
        inc/Configuration/Configuration.hpp
        inc/Configuration/ConfigurationI.hpp
        inc/Configuration/Parsers/Parser.hpp
        inc/Configuration/Parsers/ParserTypes.hpp
        inc/Configuration/Parsers/IniParser.hpp
        inc/Configuration/Parsers/JsonParser.hpp
        inc/GameAssetContainer/GameAssetContainer.hpp )

add_library(${LIBRARY_NAME} STATIC ${SRC} ${INC})

# Turns warnings ON
if(MSVC)
  target_compile_options(${LIBRARY_NAME} PRIVATE /W4 )
else()
  target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic )
endif()

# Allows Visual Studio to create a folder for the source
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${LIBRARY_NAME})

target_compile_definitions(${LIBRARY_NAME} PRIVATE PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${LIBRARY_NAME} PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")