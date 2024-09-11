cmake_minimum_required(VERSION 3.16)

set(SRC src/main.cpp  
        src/Application.cpp 
        src/SceneNode.cpp
        src/State.cpp 
        src/StateStack.cpp 
        src/MenuState.cpp
        src/GameState.cpp
        src/PauseState.cpp
        src/Message.cpp
        src/MessageNodeInfo.cpp
        src/MessageNetwork.cpp
        src/MessageNode.cpp 
        src/PlayerActionMessage.cpp
        src/TestComponentPub.cpp 
        src/TestComponentSub.cpp
        src/TestComponentSub2.cpp )

set(INC inc/Application.hpp 
        inc/SceneNode.hpp 
        inc/State.hpp
        inc/StateStack.hpp 
        inc/MenuState.hpp 
        inc/GameState.hpp
        inc/PauseState.hpp
        inc/Message.hpp
        inc/MessageNodeInfo.hpp
        inc/MessageNetwork.hpp
        inc/MessageNode.hpp
        inc/PlayerActionMessage.hpp
        inc/TestComponentPub.hpp 
        inc/TestComponentSub.hpp
        inc/TestComponentSub2.hpp )

add_executable(hch ${SRC} ${INC})

if(MSVC)
  target_compile_options(hch PRIVATE /W4 )
else()
  target_compile_options(hch PRIVATE -Wall -Wextra -Wpedantic )
endif()

target_link_libraries(hch PRIVATE sfml-graphics utility)
target_include_directories(utility PUBLIC ../lib/utility/inc inc)
target_compile_features(hch PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET hch
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:hch>
        VERBATIM)
endif()
