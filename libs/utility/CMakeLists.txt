cmake_minimum_required(VERSION 3.16)
project(utilLibrary LANGUAGES CXX)


set(SRC_UTIL src/Logger.cpp )
set(INC_UTIL inc/Logger.hpp )

add_library(utility STATIC ${SRC_UTIL} ${INC_UTIL})
target_include_directories(utility PUBLIC inc)