cmake_minimum_required(VERSION 3.30)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/app)

set(SRC src/main.cpp  
        src/Application.cpp 
        src/State/MenuState.cpp
        src/State/GameState.cpp
        src/State/PauseState.cpp )

set(INC inc/Application.hpp 
        inc/State/MenuState.hpp 
        inc/State/GameState.hpp
        inc/State/PauseState.hpp )

add_executable(hch ${SRC} ${INC})

if(MSVC)
  target_compile_options(hch PRIVATE /W4 )
else()
  target_compile_options(hch PRIVATE -Wall -Wextra -Wpedantic )
endif()

set_target_properties(hch PROPERTIES FOLDER "application")
target_link_libraries(hch PRIVATE sfml-graphics core utility)
target_include_directories(hch PUBLIC ${PROJECT_SOURCE_DIR} ${LIBS_INC_DIR})
target_compile_features(hch PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET hch
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:hch>
        VERBATIM)
endif()
