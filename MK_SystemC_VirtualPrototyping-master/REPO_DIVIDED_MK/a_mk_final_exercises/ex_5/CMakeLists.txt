cmake_minimum_required(VERSION 3.10)

project(Exercise5)

add_executable(ex_5
    main.cpp
    kpn.cpp
)

target_include_directories(ex_5
    PRIVATE $ENV{SYSTEMC_HOME}/include/
)

find_library(SYSTEMC_LIBRARY 
    NAMES systemc
    PATHS $ENV{SYSTEMC_HOME}/lib-$ENV{SYSTEMC_TARGET_ARCH}/
)

target_link_libraries(ex_5
    PRIVATE ${SYSTEMC_LIBRARY}
)
