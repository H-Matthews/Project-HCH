# Fails if the engine core depends on the game layer.
# Run by CTest as: cmake -DCORE_DIR=<libs/core> -P check_core_independence.cmake

if(NOT DEFINED CORE_DIR)
  message(FATAL_ERROR "CORE_DIR must be set")
endif()

file(GLOB_RECURSE _core_files
     "${CORE_DIR}/*.hpp" "${CORE_DIR}/*.cpp" "${CORE_DIR}/*.inl" "${CORE_DIR}/*.h")

set(_violations "")
foreach(_file ${_core_files})
  file(STRINGS "${_file}" _bad_lines REGEX "^[ \t]*#[ \t]*include[ \t]*[<\"]application/")
  if(_bad_lines)
    list(APPEND _violations "${_file}: ${_bad_lines}")
  endif()
endforeach()

if(_violations)
  string(REPLACE ";" "\n  " _report "${_violations}")
  message(FATAL_ERROR "libs/core must not include the application layer:\n  ${_report}")
endif()

list(LENGTH _core_files _count)
message(STATUS "core independence OK (${_count} files checked)")
