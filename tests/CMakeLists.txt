cmake_minimum_required(VERSION 3.30)

set(TEST_PROJECT_DIR "${CMAKE_SOURCE_DIR}")

# Glob all Test files and compile a binary for each
file(GLOB TEST_SRC_FILES 
      ${PROJECT_SOURCE_DIR}/tests/Core/*.cc
      ${PROJECT_SOURCE_DIR}/tests/Utility/*.cc
      ${PROJECT_SOURCE_DIR}/tests/Application/*.cc)
      
foreach(_test_file ${TEST_SRC_FILES})
  get_filename_component(_test_name ${_test_file} NAME_WE)
  add_executable(${_test_name} ${_test_file})
  set_target_properties(${_test_name} PROPERTIES FOLDER "tests")
  target_link_libraries(${_test_name} GTest::gtest_main engine application)
  target_compile_features(${_test_name} PRIVATE cxx_std_20)
  target_compile_definitions(${_test_name} PRIVATE
      "$<$<CONFIG:DEBUG>:APP_DEBUG>"
      "$<$<BOOL:${LOG_LEVEL_DEFINE}>:${LOG_LEVEL_DEFINE}>")

  # CTest 
  add_test(NAME ${_test_name} 
           COMMAND ${_test_name})
  set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach()


# Architecture guard: the engine must never include game code
add_test(NAME core_independence_check
         COMMAND ${CMAKE_COMMAND} -DENGINE_DIR=${PROJECT_SOURCE_DIR}/engine
                 -P ${CMAKE_CURRENT_SOURCE_DIR}/check_core_independence.cmake)
