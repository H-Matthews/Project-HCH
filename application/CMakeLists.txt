cmake_minimum_required(VERSION 3.30)

set(SRC main.cpp)

set(BINARY_NAME ${PROJECT_NAME} )

add_executable(${BINARY_NAME} ${SRC} )

if(MSVC)
  target_compile_options(${BINARY_NAME} PRIVATE /W4 )
else()
  target_compile_options(${BINARY_NAME} PRIVATE -Wall -Wextra -Wpedantic )
endif()

# Allows Visual Studio to create a folder for the source
set_target_properties(${BINARY_NAME} PROPERTIES FOLDER "application")

# Linker Options
target_link_libraries(${BINARY_NAME} PRIVATE application core utility sfml-graphics)

# Compiler Options
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${BINARY_NAME} PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")
target_include_directories(${BINARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${LIBS_INC_DIR} ${SFML_INC_DIR})

if(WIN32)
    add_custom_command(
        TARGET ${BINARY_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${BINARY_NAME}>
        VERBATIM)
endif()
