# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nouremam/WorkSpace/Exercises/Exercise2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nouremam/WorkSpace/Exercises/Exercise2/build

# Include any dependencies generated for this target.
include CMakeFiles/exor_main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/exor_main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/exor_main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/exor_main.dir/flags.make

CMakeFiles/exor_main.dir/exor_main.cpp.o: CMakeFiles/exor_main.dir/flags.make
CMakeFiles/exor_main.dir/exor_main.cpp.o: /home/nouremam/WorkSpace/Exercises/Exercise2/exor_main.cpp
CMakeFiles/exor_main.dir/exor_main.cpp.o: CMakeFiles/exor_main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/nouremam/WorkSpace/Exercises/Exercise2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/exor_main.dir/exor_main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exor_main.dir/exor_main.cpp.o -MF CMakeFiles/exor_main.dir/exor_main.cpp.o.d -o CMakeFiles/exor_main.dir/exor_main.cpp.o -c /home/nouremam/WorkSpace/Exercises/Exercise2/exor_main.cpp

CMakeFiles/exor_main.dir/exor_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/exor_main.dir/exor_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nouremam/WorkSpace/Exercises/Exercise2/exor_main.cpp > CMakeFiles/exor_main.dir/exor_main.cpp.i

CMakeFiles/exor_main.dir/exor_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/exor_main.dir/exor_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nouremam/WorkSpace/Exercises/Exercise2/exor_main.cpp -o CMakeFiles/exor_main.dir/exor_main.cpp.s

# Object files for target exor_main
exor_main_OBJECTS = \
"CMakeFiles/exor_main.dir/exor_main.cpp.o"

# External object files for target exor_main
exor_main_EXTERNAL_OBJECTS =

exor_main: CMakeFiles/exor_main.dir/exor_main.cpp.o
exor_main: CMakeFiles/exor_main.dir/build.make
exor_main: CMakeFiles/exor_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/nouremam/WorkSpace/Exercises/Exercise2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable exor_main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/exor_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/exor_main.dir/build: exor_main
.PHONY : CMakeFiles/exor_main.dir/build

CMakeFiles/exor_main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exor_main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exor_main.dir/clean

CMakeFiles/exor_main.dir/depend:
	cd /home/nouremam/WorkSpace/Exercises/Exercise2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nouremam/WorkSpace/Exercises/Exercise2 /home/nouremam/WorkSpace/Exercises/Exercise2 /home/nouremam/WorkSpace/Exercises/Exercise2/build /home/nouremam/WorkSpace/Exercises/Exercise2/build /home/nouremam/WorkSpace/Exercises/Exercise2/build/CMakeFiles/exor_main.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/exor_main.dir/depend
