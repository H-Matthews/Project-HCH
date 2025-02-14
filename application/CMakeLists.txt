cmake_minimum_required(VERSION 3.30)

set(SRC main.cpp)

set(BINARY_NAME ${PROJECT_NAME} )

add_executable(${BINARY_NAME} ${SRC} )

# Allows Visual Studio to create a folder for the source
set_target_properties(${BINARY_NAME} PROPERTIES FOLDER ${BINARY_NAME})

# Linker Options
target_link_libraries(${BINARY_NAME} PRIVATE application)

# Compiler Options
target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${BINARY_NAME} PRIVATE "$<$<CONFIG:DEBUG>:APP_DEBUG>")
target_include_directories(${BINARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${LIBS_INC_DIR} ${SFML_INC_DIR})
