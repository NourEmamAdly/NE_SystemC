cmake_minimum_required(VERSION 3.10)

project(Basic)

add_executable(basic_module
    basic_module.cpp
    
)

target_include_directories(basic_module
    PRIVATE $ENV{SYSTEMC_HOME}/include/
)

find_library(SYSTEMC_LIBRARY 
    NAMES systemc
    PATHS $ENV{SYSTEMC_HOME}/lib-$ENV{SYSTEMC_TARGET_ARCH}/
)

target_link_libraries(basic_module
    PRIVATE ${SYSTEMC_LIBRARY}
)
