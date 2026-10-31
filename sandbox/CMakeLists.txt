cmake_minimum_required(VERSION 3.30)

# Minimal executable linking only the engine, for trying out engine features without game code
add_executable(Sandbox main.cpp)
set_target_properties(Sandbox PROPERTIES FOLDER sandbox)
target_link_libraries(Sandbox PRIVATE engine)
target_compile_definitions(Sandbox PRIVATE SANDBOX_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
