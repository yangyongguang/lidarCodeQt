# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yyg/code/lidarCode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yyg/code/lidarCode/build

# Include any dependencies generated for this target.
include src/groundRemove/CMakeFiles/segment.dir/depend.make

# Include the progress variables for this target.
include src/groundRemove/CMakeFiles/segment.dir/progress.make

# Include the compile flags for this target's objects.
include src/groundRemove/CMakeFiles/segment.dir/flags.make

src/groundRemove/CMakeFiles/segment.dir/src/segment.cpp.o: src/groundRemove/CMakeFiles/segment.dir/flags.make
src/groundRemove/CMakeFiles/segment.dir/src/segment.cpp.o: ../src/groundRemove/src/segment.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yyg/code/lidarCode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/groundRemove/CMakeFiles/segment.dir/src/segment.cpp.o"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/segment.dir/src/segment.cpp.o -c /home/yyg/code/lidarCode/src/groundRemove/src/segment.cpp

src/groundRemove/CMakeFiles/segment.dir/src/segment.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/src/segment.cpp.i"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yyg/code/lidarCode/src/groundRemove/src/segment.cpp > CMakeFiles/segment.dir/src/segment.cpp.i

src/groundRemove/CMakeFiles/segment.dir/src/segment.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/src/segment.cpp.s"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yyg/code/lidarCode/src/groundRemove/src/segment.cpp -o CMakeFiles/segment.dir/src/segment.cpp.s

# Object files for target segment
segment_OBJECTS = \
"CMakeFiles/segment.dir/src/segment.cpp.o"

# External object files for target segment
segment_EXTERNAL_OBJECTS =

src/groundRemove/libsegment.so: src/groundRemove/CMakeFiles/segment.dir/src/segment.cpp.o
src/groundRemove/libsegment.so: src/groundRemove/CMakeFiles/segment.dir/build.make
src/groundRemove/libsegment.so: src/groundRemove/CMakeFiles/segment.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yyg/code/lidarCode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libsegment.so"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/segment.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/groundRemove/CMakeFiles/segment.dir/build: src/groundRemove/libsegment.so

.PHONY : src/groundRemove/CMakeFiles/segment.dir/build

src/groundRemove/CMakeFiles/segment.dir/clean:
	cd /home/yyg/code/lidarCode/build/src/groundRemove && $(CMAKE_COMMAND) -P CMakeFiles/segment.dir/cmake_clean.cmake
.PHONY : src/groundRemove/CMakeFiles/segment.dir/clean

src/groundRemove/CMakeFiles/segment.dir/depend:
	cd /home/yyg/code/lidarCode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yyg/code/lidarCode /home/yyg/code/lidarCode/src/groundRemove /home/yyg/code/lidarCode/build /home/yyg/code/lidarCode/build/src/groundRemove /home/yyg/code/lidarCode/build/src/groundRemove/CMakeFiles/segment.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/groundRemove/CMakeFiles/segment.dir/depend

