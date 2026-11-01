# Fails if the engine depends on a game layer.
# Run by CTest as: cmake -DENGINE_DIR=<engine> -P check_core_independence.cmake

if(NOT DEFINED ENGINE_DIR)
  message(FATAL_ERROR "ENGINE_DIR must be set")
endif()

file(GLOB_RECURSE _engine_files
     "${ENGINE_DIR}/*.hpp" "${ENGINE_DIR}/*.cpp" "${ENGINE_DIR}/*.inl" "${ENGINE_DIR}/*.h")
# Vendored third-party headers are not ours to police
list(FILTER _engine_files EXCLUDE REGEX "/vendor/")

set(_violations "")
foreach(_file ${_engine_files})
  file(STRINGS "${_file}" _bad_lines
       REGEX "^[ \t]*#[ \t]*include[ \t]*[<\"](application|games)/")
  if(_bad_lines)
    list(APPEND _violations "${_file}: ${_bad_lines}")
  endif()
endforeach()

if(_violations)
  string(REPLACE ";" "\n  " _report "${_violations}")
  message(FATAL_ERROR "engine/ must not include game code (application/ or games/):\n  ${_report}")
endif()

list(LENGTH _engine_files _count)
message(STATUS "engine independence OK (${_count} files checked)")
