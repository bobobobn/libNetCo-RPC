# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/bob/libNetCo-RPC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bob/libNetCo-RPC

# Include any dependencies generated for this target.
include CMakeFiles/zkTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/zkTest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/zkTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zkTest.dir/flags.make

CMakeFiles/zkTest.dir/example/zkTest.o: CMakeFiles/zkTest.dir/flags.make
CMakeFiles/zkTest.dir/example/zkTest.o: example/zkTest.cc
CMakeFiles/zkTest.dir/example/zkTest.o: CMakeFiles/zkTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bob/libNetCo-RPC/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/zkTest.dir/example/zkTest.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/zkTest.dir/example/zkTest.o -MF CMakeFiles/zkTest.dir/example/zkTest.o.d -o CMakeFiles/zkTest.dir/example/zkTest.o -c /home/bob/libNetCo-RPC/example/zkTest.cc

CMakeFiles/zkTest.dir/example/zkTest.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zkTest.dir/example/zkTest.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bob/libNetCo-RPC/example/zkTest.cc > CMakeFiles/zkTest.dir/example/zkTest.i

CMakeFiles/zkTest.dir/example/zkTest.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zkTest.dir/example/zkTest.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bob/libNetCo-RPC/example/zkTest.cc -o CMakeFiles/zkTest.dir/example/zkTest.s

# Object files for target zkTest
zkTest_OBJECTS = \
"CMakeFiles/zkTest.dir/example/zkTest.o"

# External object files for target zkTest
zkTest_EXTERNAL_OBJECTS =

bin/zkTest: CMakeFiles/zkTest.dir/example/zkTest.o
bin/zkTest: CMakeFiles/zkTest.dir/build.make
bin/zkTest: liblibnetco.so
bin/zkTest: /usr/local/lib/libprotobuf.so
bin/zkTest: /usr/local/lib/libzookeeper_mt.so
bin/zkTest: CMakeFiles/zkTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bob/libNetCo-RPC/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/zkTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zkTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zkTest.dir/build: bin/zkTest
.PHONY : CMakeFiles/zkTest.dir/build

CMakeFiles/zkTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zkTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zkTest.dir/clean

CMakeFiles/zkTest.dir/depend:
	cd /home/bob/libNetCo-RPC && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bob/libNetCo-RPC /home/bob/libNetCo-RPC /home/bob/libNetCo-RPC /home/bob/libNetCo-RPC /home/bob/libNetCo-RPC/CMakeFiles/zkTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zkTest.dir/depend

