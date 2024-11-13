cmake_minimum_required(VERSION 3.30)
project(coreLibrary LANGUAGES CXX)


# Top Level directory definition
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")

set(SRC src/Messaging/Message.cpp
        src/Messaging/MessageNetwork.cpp
        src/Messaging/MessageNode.cpp
        src/Messaging/MessageNodeInfo.cpp
        src/State/State.cpp 
        src/State/StateStack.cpp
        src/SceneNode.cpp
        src/Configuration.cpp )

set(INC inc/Messaging/Message.hpp
        inc/Messaging/MessageNetwork.hpp
        inc/Messaging/MessageNode.hpp
        inc/Messaging/MessageNodeInfo.hpp
        inc/State/State.hpp 
        inc/State/StateStack.hpp
        inc/SceneNode.hpp
        inc/Configuration.hpp
        inc/ConfigurationI.hpp )

add_library(core STATIC ${SRC} ${INC})

target_compile_definitions(core PRIVATE PROJECT_DIR="${PROJECT_DIR}")
target_include_directories(core PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(core PRIVATE cxx_std_17)