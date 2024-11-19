cmake_minimum_required(VERSION 3.30)
project(utilLibrary LANGUAGES CXX)


set(SRC src/ConsoleLogger.cpp
        src/TextFileLogger.cpp
        src/LogRegistry.cpp
        src/StringOperations.cpp )

set(INC inc/LoggerI.hpp
        inc/ConsoleLogger.hpp
        inc/TextFileLogger.hpp
        inc/LogRegistry.hpp
        inc/StringOperations.hpp )

add_library(utility STATIC ${SRC} ${INC})
target_include_directories(utility PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(utility PRIVATE cxx_std_17)