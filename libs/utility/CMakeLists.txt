cmake_minimum_required(VERSION 3.30)
project(utilLibrary LANGUAGES CXX)



set(SRC src/Logging/LogRegistry.cpp 
        src/Logging/Sinks/ColorConsoleSink.cpp
        src/Logging/Sinks/TextFileSink.cpp
        src/Logging/Logger.cpp
        src/Logging/LogLevel.cpp )

set(INC inc/Logging/LogRegistry.hpp
        inc/Logging/Sinks/LogSinksI.hpp
        inc/Logging/Sinks/ColorConsoleSink.hpp
        inc/Logging/Sinks/TextFileSink.hpp
        inc/Logging/Logger.hpp
        inc/StringOperations.hpp
        inc/Logging/LogLevel.hpp )

add_library(utility STATIC ${SRC} ${INC})

set_target_properties(utility PROPERTIES FOLDER "utility")
target_include_directories(utility PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(utility PRIVATE cxx_std_20)