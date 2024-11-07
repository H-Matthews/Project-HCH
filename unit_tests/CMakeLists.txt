cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../unit_tests/bin)

enable_testing()

set(TEST_FILES_SRC hello_test.cc)

add_executable(hello_test ${TEST_FILES_SRC})

if(MSVC)
  target_compile_options(hello_test PRIVATE /W4 )
else()
  target_compile_options(hello_test PRIVATE -Wall -Wextra -Wpedantic )
endif()

target_link_libraries(hello_test GTest::gtest_main)
target_compile_features(hello_test PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET hello_test
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:hello_test>
        VERBATIM)
endif()
