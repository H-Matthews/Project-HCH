cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/tests)

# Glob all Test files and compile a binary for each
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cc)
foreach(_test_file ${TEST_SRC_FILES})
  get_filename_component(_test_name ${_test_file} NAME_WE)
  add_executable(${_test_name} ${_test_file})
  target_include_directories(${_test_name} PUBLIC ${LIBS_INC_DIR})
  target_link_libraries(${_test_name} GTest::gtest_main utility)
  target_compile_features(${_test_name} PRIVATE cxx_std_17)

  # CTest 
  add_test(NAME ${_test_name} 
           COMMAND ${_test_name})
  set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)

  if(WIN32)
    add_custom_command(
        TARGET ${_test_name}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${_test_name}>
        VERBATIM)
  endif()
endforeach()
