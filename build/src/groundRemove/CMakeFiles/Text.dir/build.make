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
include src/groundRemove/CMakeFiles/Text.dir/depend.make

# Include the progress variables for this target.
include src/groundRemove/CMakeFiles/Text.dir/progress.make

# Include the compile flags for this target's objects.
include src/groundRemove/CMakeFiles/Text.dir/flags.make

src/groundRemove/CMakeFiles/Text.dir/src/test.cpp.o: src/groundRemove/CMakeFiles/Text.dir/flags.make
src/groundRemove/CMakeFiles/Text.dir/src/test.cpp.o: ../src/groundRemove/src/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yyg/code/lidarCode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/groundRemove/CMakeFiles/Text.dir/src/test.cpp.o"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Text.dir/src/test.cpp.o -c /home/yyg/code/lidarCode/src/groundRemove/src/test.cpp

src/groundRemove/CMakeFiles/Text.dir/src/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Text.dir/src/test.cpp.i"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yyg/code/lidarCode/src/groundRemove/src/test.cpp > CMakeFiles/Text.dir/src/test.cpp.i

src/groundRemove/CMakeFiles/Text.dir/src/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Text.dir/src/test.cpp.s"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yyg/code/lidarCode/src/groundRemove/src/test.cpp -o CMakeFiles/Text.dir/src/test.cpp.s

# Object files for target Text
Text_OBJECTS = \
"CMakeFiles/Text.dir/src/test.cpp.o"

# External object files for target Text
Text_EXTERNAL_OBJECTS =

src/groundRemove/Text: src/groundRemove/CMakeFiles/Text.dir/src/test.cpp.o
src/groundRemove/Text: src/groundRemove/CMakeFiles/Text.dir/build.make
src/groundRemove/Text: src/groundRemove/CMakeFiles/Text.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yyg/code/lidarCode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Text"
	cd /home/yyg/code/lidarCode/build/src/groundRemove && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Text.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/groundRemove/CMakeFiles/Text.dir/build: src/groundRemove/Text

.PHONY : src/groundRemove/CMakeFiles/Text.dir/build

src/groundRemove/CMakeFiles/Text.dir/clean:
	cd /home/yyg/code/lidarCode/build/src/groundRemove && $(CMAKE_COMMAND) -P CMakeFiles/Text.dir/cmake_clean.cmake
.PHONY : src/groundRemove/CMakeFiles/Text.dir/clean

src/groundRemove/CMakeFiles/Text.dir/depend:
	cd /home/yyg/code/lidarCode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yyg/code/lidarCode /home/yyg/code/lidarCode/src/groundRemove /home/yyg/code/lidarCode/build /home/yyg/code/lidarCode/build/src/groundRemove /home/yyg/code/lidarCode/build/src/groundRemove/CMakeFiles/Text.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/groundRemove/CMakeFiles/Text.dir/depend
