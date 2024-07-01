cmake_minimum_required(VERSION 3.10)
project(Exercise3 VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Path to the SystemC installation
set(SYSTEMC_HOME /usr/local/systemc)

# Include directories for SystemC
include_directories(${SYSTEMC_HOME}/include)

# Assuming SystemC libraries are in the lib-linux64 directory under SYSTEMC_HOME
link_directories(${SYSTEMC_HOME}/lib-linux64)

# Add executable target with source files

add_executable(state_machine state_machine.cpp stimuli stimuli.cpp main main.cpp)



# Link the SystemC library
target_link_libraries(state_machine systemc)

# Optional: Specify include directories for the target
#target_include_directories(state_machine PRIVATE ${PROJECT_SOURCE_DIR}/include)