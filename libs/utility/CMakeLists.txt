cmake_minimum_required(VERSION 3.16)
project(utilLibrary LANGUAGES CXX)


set(SRC src/Logger.cpp 
        src/StringOperations.cpp )

set(INC inc/Logger.hpp 
        inc/StringOperations.hpp )

add_library(utility STATIC ${SRC} ${INC})
target_include_directories(utility PUBLIC ${SFML_INC_DIR} ../)
target_compile_features(utility PRIVATE cxx_std_17)