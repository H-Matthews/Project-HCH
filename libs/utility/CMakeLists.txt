cmake_minimum_required(VERSION 3.30)
project(utilLibrary LANGUAGES CXX)



set(SRC src/Logging/Sinks/ColorConsoleSink.cpp
        src/Logging/Logger.cpp
        src/Logging/LogRegistry.cpp
        src/StringOperations.cpp )

set(INC inc/Logging/Sinks/LogSinksI.hpp
        inc/Logging/Sinks/ColorConsoleSink.hpp
        inc/Logging/Logger.hpp
        inc/Logging/LogRegistry.hpp
        inc/StringOperations.hpp
        inc/Logging/CommonLog.hpp )

add_library(utility STATIC ${SRC} ${INC})

set_target_properties(utility PROPERTIES FOLDER "utility")
target_include_directories(utility PUBLIC ${SFML_INC_DIR} ${LIBS_INC_DIR})
target_compile_features(utility PRIVATE cxx_std_17)