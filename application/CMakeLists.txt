cmake_minimum_required(VERSION 3.16)

set(SRC src/main.cpp  src/Application.cpp)
set(INC inc/Application.hpp)

add_executable(hch ${SRC} ${INC})

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
