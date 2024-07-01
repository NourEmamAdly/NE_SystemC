cmake_minimum_required(VERSION 3.10)

project(Exercise3)

add_executable(ex_3
    main.cpp
    state_machine.cpp
    stimuli.cpp
)

target_include_directories(ex_3
    PRIVATE $ENV{SYSTEMC_HOME}/include/
)

find_library(SYSTEMC_LIBRARY 
    NAMES systemc
    PATHS $ENV{SYSTEMC_HOME}/lib-$ENV{SYSTEMC_TARGET_ARCH}/
)

target_link_libraries(ex_3
    PRIVATE ${SYSTEMC_LIBRARY}
)
