cmake_minimum_required(VERSION 3.10)
project(03-CustomSignal VERSION 1.0 LANGUAGES CXX)

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

add_executable(main main.cpp)



# Link the SystemC library
target_link_libraries(main systemc)

# Optional: Specify include directories for the target
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)
