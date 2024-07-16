# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_SOURCE_DIR = /home/bob/libNetCo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bob/libNetCo

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bob/libNetCo/CMakeFiles /home/bob/libNetCo//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bob/libNetCo/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named libnetco

# Build rule for target.
libnetco: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 libnetco
.PHONY : libnetco

# fast build rule for target.
libnetco/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/build
.PHONY : libnetco/fast

#=============================================================================
# Target rules for targets named doubly_buffer_test

# Build rule for target.
doubly_buffer_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 doubly_buffer_test
.PHONY : doubly_buffer_test

# fast build rule for target.
doubly_buffer_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/doubly_buffer_test.dir/build.make CMakeFiles/doubly_buffer_test.dir/build
.PHONY : doubly_buffer_test/fast

#=============================================================================
# Target rules for targets named log_test

# Build rule for target.
log_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 log_test
.PHONY : log_test

# fast build rule for target.
log_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/log_test.dir/build.make CMakeFiles/log_test.dir/build
.PHONY : log_test/fast

#=============================================================================
# Target rules for targets named method_test

# Build rule for target.
method_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 method_test
.PHONY : method_test

# fast build rule for target.
method_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/method_test.dir/build.make CMakeFiles/method_test.dir/build
.PHONY : method_test/fast

#=============================================================================
# Target rules for targets named name_service_test

# Build rule for target.
name_service_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 name_service_test
.PHONY : name_service_test

# fast build rule for target.
name_service_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/name_service_test.dir/build.make CMakeFiles/name_service_test.dir/build
.PHONY : name_service_test/fast

#=============================================================================
# Target rules for targets named rpc_client_pb_test

# Build rule for target.
rpc_client_pb_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 rpc_client_pb_test
.PHONY : rpc_client_pb_test

# fast build rule for target.
rpc_client_pb_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_client_pb_test.dir/build.make CMakeFiles/rpc_client_pb_test.dir/build
.PHONY : rpc_client_pb_test/fast

#=============================================================================
# Target rules for targets named rpc_server_pb_test

# Build rule for target.
rpc_server_pb_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 rpc_server_pb_test
.PHONY : rpc_server_pb_test

# fast build rule for target.
rpc_server_pb_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_server_pb_test.dir/build.make CMakeFiles/rpc_server_pb_test.dir/build
.PHONY : rpc_server_pb_test/fast

#=============================================================================
# Target rules for targets named tcp_client_test

# Build rule for target.
tcp_client_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 tcp_client_test
.PHONY : tcp_client_test

# fast build rule for target.
tcp_client_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_client_test.dir/build.make CMakeFiles/tcp_client_test.dir/build
.PHONY : tcp_client_test/fast

#=============================================================================
# Target rules for targets named tcp_server_test

# Build rule for target.
tcp_server_test: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 tcp_server_test
.PHONY : tcp_server_test

# fast build rule for target.
tcp_server_test/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_server_test.dir/build.make CMakeFiles/tcp_server_test.dir/build
.PHONY : tcp_server_test/fast

#=============================================================================
# Target rules for targets named zkTest

# Build rule for target.
zkTest: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 zkTest
.PHONY : zkTest

# fast build rule for target.
zkTest/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/zkTest.dir/build.make CMakeFiles/zkTest.dir/build
.PHONY : zkTest/fast

# target to build an object file
example/doubly_buffer_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/doubly_buffer_test.dir/build.make CMakeFiles/doubly_buffer_test.dir/example/doubly_buffer_test.o
.PHONY : example/doubly_buffer_test.o

# target to preprocess a source file
example/doubly_buffer_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/doubly_buffer_test.dir/build.make CMakeFiles/doubly_buffer_test.dir/example/doubly_buffer_test.i
.PHONY : example/doubly_buffer_test.i

# target to generate assembly for a file
example/doubly_buffer_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/doubly_buffer_test.dir/build.make CMakeFiles/doubly_buffer_test.dir/example/doubly_buffer_test.s
.PHONY : example/doubly_buffer_test.s

# target to build an object file
example/log_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/log_test.dir/build.make CMakeFiles/log_test.dir/example/log_test.o
.PHONY : example/log_test.o

# target to preprocess a source file
example/log_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/log_test.dir/build.make CMakeFiles/log_test.dir/example/log_test.i
.PHONY : example/log_test.i

# target to generate assembly for a file
example/log_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/log_test.dir/build.make CMakeFiles/log_test.dir/example/log_test.s
.PHONY : example/log_test.s

# target to build an object file
example/method_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/method_test.dir/build.make CMakeFiles/method_test.dir/example/method_test.o
.PHONY : example/method_test.o

# target to preprocess a source file
example/method_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/method_test.dir/build.make CMakeFiles/method_test.dir/example/method_test.i
.PHONY : example/method_test.i

# target to generate assembly for a file
example/method_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/method_test.dir/build.make CMakeFiles/method_test.dir/example/method_test.s
.PHONY : example/method_test.s

# target to build an object file
example/name_service_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/name_service_test.dir/build.make CMakeFiles/name_service_test.dir/example/name_service_test.o
.PHONY : example/name_service_test.o

# target to preprocess a source file
example/name_service_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/name_service_test.dir/build.make CMakeFiles/name_service_test.dir/example/name_service_test.i
.PHONY : example/name_service_test.i

# target to generate assembly for a file
example/name_service_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/name_service_test.dir/build.make CMakeFiles/name_service_test.dir/example/name_service_test.s
.PHONY : example/name_service_test.s

# target to build an object file
example/rpc_client_pb_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_client_pb_test.dir/build.make CMakeFiles/rpc_client_pb_test.dir/example/rpc_client_pb_test.o
.PHONY : example/rpc_client_pb_test.o

# target to preprocess a source file
example/rpc_client_pb_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_client_pb_test.dir/build.make CMakeFiles/rpc_client_pb_test.dir/example/rpc_client_pb_test.i
.PHONY : example/rpc_client_pb_test.i

# target to generate assembly for a file
example/rpc_client_pb_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_client_pb_test.dir/build.make CMakeFiles/rpc_client_pb_test.dir/example/rpc_client_pb_test.s
.PHONY : example/rpc_client_pb_test.s

# target to build an object file
example/rpc_server_pb_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_server_pb_test.dir/build.make CMakeFiles/rpc_server_pb_test.dir/example/rpc_server_pb_test.o
.PHONY : example/rpc_server_pb_test.o

# target to preprocess a source file
example/rpc_server_pb_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_server_pb_test.dir/build.make CMakeFiles/rpc_server_pb_test.dir/example/rpc_server_pb_test.i
.PHONY : example/rpc_server_pb_test.i

# target to generate assembly for a file
example/rpc_server_pb_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/rpc_server_pb_test.dir/build.make CMakeFiles/rpc_server_pb_test.dir/example/rpc_server_pb_test.s
.PHONY : example/rpc_server_pb_test.s

# target to build an object file
example/tcp_client_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_client_test.dir/build.make CMakeFiles/tcp_client_test.dir/example/tcp_client_test.o
.PHONY : example/tcp_client_test.o

# target to preprocess a source file
example/tcp_client_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_client_test.dir/build.make CMakeFiles/tcp_client_test.dir/example/tcp_client_test.i
.PHONY : example/tcp_client_test.i

# target to generate assembly for a file
example/tcp_client_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_client_test.dir/build.make CMakeFiles/tcp_client_test.dir/example/tcp_client_test.s
.PHONY : example/tcp_client_test.s

# target to build an object file
example/tcp_server_test.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_server_test.dir/build.make CMakeFiles/tcp_server_test.dir/example/tcp_server_test.o
.PHONY : example/tcp_server_test.o

# target to preprocess a source file
example/tcp_server_test.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_server_test.dir/build.make CMakeFiles/tcp_server_test.dir/example/tcp_server_test.i
.PHONY : example/tcp_server_test.i

# target to generate assembly for a file
example/tcp_server_test.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tcp_server_test.dir/build.make CMakeFiles/tcp_server_test.dir/example/tcp_server_test.s
.PHONY : example/tcp_server_test.s

# target to build an object file
example/zkTest.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/zkTest.dir/build.make CMakeFiles/zkTest.dir/example/zkTest.o
.PHONY : example/zkTest.o

# target to preprocess a source file
example/zkTest.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/zkTest.dir/build.make CMakeFiles/zkTest.dir/example/zkTest.i
.PHONY : example/zkTest.i

# target to generate assembly for a file
example/zkTest.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/zkTest.dir/build.make CMakeFiles/zkTest.dir/example/zkTest.s
.PHONY : example/zkTest.s

# target to build an object file
src/context.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/context.o
.PHONY : src/context.o

# target to preprocess a source file
src/context.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/context.i
.PHONY : src/context.i

# target to generate assembly for a file
src/context.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/context.s
.PHONY : src/context.s

# target to build an object file
src/coroutine.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/coroutine.o
.PHONY : src/coroutine.o

# target to preprocess a source file
src/coroutine.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/coroutine.i
.PHONY : src/coroutine.i

# target to generate assembly for a file
src/coroutine.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/coroutine.s
.PHONY : src/coroutine.s

# target to build an object file
src/epoller.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/epoller.o
.PHONY : src/epoller.o

# target to preprocess a source file
src/epoller.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/epoller.i
.PHONY : src/epoller.i

# target to generate assembly for a file
src/epoller.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/epoller.s
.PHONY : src/epoller.s

# target to build an object file
src/log.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/log.o
.PHONY : src/log.o

# target to preprocess a source file
src/log.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/log.i
.PHONY : src/log.i

# target to generate assembly for a file
src/log.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/log.s
.PHONY : src/log.s

# target to build an object file
src/mstime.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/mstime.o
.PHONY : src/mstime.o

# target to preprocess a source file
src/mstime.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/mstime.i
.PHONY : src/mstime.i

# target to generate assembly for a file
src/mstime.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/mstime.s
.PHONY : src/mstime.s

# target to build an object file
src/netco_api.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/netco_api.o
.PHONY : src/netco_api.o

# target to preprocess a source file
src/netco_api.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/netco_api.i
.PHONY : src/netco_api.i

# target to generate assembly for a file
src/netco_api.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/netco_api.s
.PHONY : src/netco_api.s

# target to build an object file
src/processor.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/processor.o
.PHONY : src/processor.o

# target to preprocess a source file
src/processor.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/processor.i
.PHONY : src/processor.i

# target to generate assembly for a file
src/processor.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/processor.s
.PHONY : src/processor.s

# target to build an object file
src/processor_selector.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/processor_selector.o
.PHONY : src/processor_selector.o

# target to preprocess a source file
src/processor_selector.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/processor_selector.i
.PHONY : src/processor_selector.i

# target to generate assembly for a file
src/processor_selector.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/processor_selector.s
.PHONY : src/processor_selector.s

# target to build an object file
src/rpc_proto/int_message.pb.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/int_message.pb.o
.PHONY : src/rpc_proto/int_message.pb.o

# target to preprocess a source file
src/rpc_proto/int_message.pb.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/int_message.pb.i
.PHONY : src/rpc_proto/int_message.pb.i

# target to generate assembly for a file
src/rpc_proto/int_message.pb.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/int_message.pb.s
.PHONY : src/rpc_proto/int_message.pb.s

# target to build an object file
src/rpc_proto/load_balancer.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/load_balancer.o
.PHONY : src/rpc_proto/load_balancer.o

# target to preprocess a source file
src/rpc_proto/load_balancer.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/load_balancer.i
.PHONY : src/rpc_proto/load_balancer.i

# target to generate assembly for a file
src/rpc_proto/load_balancer.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/load_balancer.s
.PHONY : src/rpc_proto/load_balancer.s

# target to build an object file
src/rpc_proto/locality_aware_load_balancer.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/locality_aware_load_balancer.o
.PHONY : src/rpc_proto/locality_aware_load_balancer.o

# target to preprocess a source file
src/rpc_proto/locality_aware_load_balancer.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/locality_aware_load_balancer.i
.PHONY : src/rpc_proto/locality_aware_load_balancer.i

# target to generate assembly for a file
src/rpc_proto/locality_aware_load_balancer.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/locality_aware_load_balancer.s
.PHONY : src/rpc_proto/locality_aware_load_balancer.s

# target to build an object file
src/rpc_proto/rpc_client.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_client.o
.PHONY : src/rpc_proto/rpc_client.o

# target to preprocess a source file
src/rpc_proto/rpc_client.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_client.i
.PHONY : src/rpc_proto/rpc_client.i

# target to generate assembly for a file
src/rpc_proto/rpc_client.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_client.s
.PHONY : src/rpc_proto/rpc_client.s

# target to build an object file
src/rpc_proto/rpc_client_stub.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_client_stub.o
.PHONY : src/rpc_proto/rpc_client_stub.o

# target to preprocess a source file
src/rpc_proto/rpc_client_stub.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_client_stub.i
.PHONY : src/rpc_proto/rpc_client_stub.i

# target to generate assembly for a file
src/rpc_proto/rpc_client_stub.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_client_stub.s
.PHONY : src/rpc_proto/rpc_client_stub.s

# target to build an object file
src/rpc_proto/rpc_response_header.pb.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_response_header.pb.o
.PHONY : src/rpc_proto/rpc_response_header.pb.o

# target to preprocess a source file
src/rpc_proto/rpc_response_header.pb.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_response_header.pb.i
.PHONY : src/rpc_proto/rpc_response_header.pb.i

# target to generate assembly for a file
src/rpc_proto/rpc_response_header.pb.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_response_header.pb.s
.PHONY : src/rpc_proto/rpc_response_header.pb.s

# target to build an object file
src/rpc_proto/rpc_server.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_server.o
.PHONY : src/rpc_proto/rpc_server.o

# target to preprocess a source file
src/rpc_proto/rpc_server.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_server.i
.PHONY : src/rpc_proto/rpc_server.i

# target to generate assembly for a file
src/rpc_proto/rpc_server.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_server.s
.PHONY : src/rpc_proto/rpc_server.s

# target to build an object file
src/rpc_proto/rpc_server_stub.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_server_stub.o
.PHONY : src/rpc_proto/rpc_server_stub.o

# target to preprocess a source file
src/rpc_proto/rpc_server_stub.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_server_stub.i
.PHONY : src/rpc_proto/rpc_server_stub.i

# target to generate assembly for a file
src/rpc_proto/rpc_server_stub.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpc_server_stub.s
.PHONY : src/rpc_proto/rpc_server_stub.s

# target to build an object file
src/rpc_proto/rpcheader.pb.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpcheader.pb.o
.PHONY : src/rpc_proto/rpcheader.pb.o

# target to preprocess a source file
src/rpc_proto/rpcheader.pb.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpcheader.pb.i
.PHONY : src/rpc_proto/rpcheader.pb.i

# target to generate assembly for a file
src/rpc_proto/rpcheader.pb.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/rpcheader.pb.s
.PHONY : src/rpc_proto/rpcheader.pb.s

# target to build an object file
src/rpc_proto/zk_naming_service.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/zk_naming_service.o
.PHONY : src/rpc_proto/zk_naming_service.o

# target to preprocess a source file
src/rpc_proto/zk_naming_service.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/zk_naming_service.i
.PHONY : src/rpc_proto/zk_naming_service.i

# target to generate assembly for a file
src/rpc_proto/zk_naming_service.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/rpc_proto/zk_naming_service.s
.PHONY : src/rpc_proto/zk_naming_service.s

# target to build an object file
src/scheduler.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/scheduler.o
.PHONY : src/scheduler.o

# target to preprocess a source file
src/scheduler.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/scheduler.i
.PHONY : src/scheduler.i

# target to generate assembly for a file
src/scheduler.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/scheduler.s
.PHONY : src/scheduler.s

# target to build an object file
src/socket.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/socket.o
.PHONY : src/socket.o

# target to preprocess a source file
src/socket.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/socket.i
.PHONY : src/socket.i

# target to generate assembly for a file
src/socket.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/socket.s
.PHONY : src/socket.s

# target to build an object file
src/tcp/tcp_client.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/tcp/tcp_client.o
.PHONY : src/tcp/tcp_client.o

# target to preprocess a source file
src/tcp/tcp_client.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/tcp/tcp_client.i
.PHONY : src/tcp/tcp_client.i

# target to generate assembly for a file
src/tcp/tcp_client.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/tcp/tcp_client.s
.PHONY : src/tcp/tcp_client.s

# target to build an object file
src/tcp/tcp_server.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/tcp/tcp_server.o
.PHONY : src/tcp/tcp_server.o

# target to preprocess a source file
src/tcp/tcp_server.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/tcp/tcp_server.i
.PHONY : src/tcp/tcp_server.i

# target to generate assembly for a file
src/tcp/tcp_server.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/tcp/tcp_server.s
.PHONY : src/tcp/tcp_server.s

# target to build an object file
src/timer.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/timer.o
.PHONY : src/timer.o

# target to preprocess a source file
src/timer.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/timer.i
.PHONY : src/timer.i

# target to generate assembly for a file
src/timer.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/timer.s
.PHONY : src/timer.s

# target to build an object file
src/zk_client.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/zk_client.o
.PHONY : src/zk_client.o

# target to preprocess a source file
src/zk_client.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/zk_client.i
.PHONY : src/zk_client.i

# target to generate assembly for a file
src/zk_client.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/libnetco.dir/build.make CMakeFiles/libnetco.dir/src/zk_client.s
.PHONY : src/zk_client.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... doubly_buffer_test"
	@echo "... libnetco"
	@echo "... log_test"
	@echo "... method_test"
	@echo "... name_service_test"
	@echo "... rpc_client_pb_test"
	@echo "... rpc_server_pb_test"
	@echo "... tcp_client_test"
	@echo "... tcp_server_test"
	@echo "... zkTest"
	@echo "... example/doubly_buffer_test.o"
	@echo "... example/doubly_buffer_test.i"
	@echo "... example/doubly_buffer_test.s"
	@echo "... example/log_test.o"
	@echo "... example/log_test.i"
	@echo "... example/log_test.s"
	@echo "... example/method_test.o"
	@echo "... example/method_test.i"
	@echo "... example/method_test.s"
	@echo "... example/name_service_test.o"
	@echo "... example/name_service_test.i"
	@echo "... example/name_service_test.s"
	@echo "... example/rpc_client_pb_test.o"
	@echo "... example/rpc_client_pb_test.i"
	@echo "... example/rpc_client_pb_test.s"
	@echo "... example/rpc_server_pb_test.o"
	@echo "... example/rpc_server_pb_test.i"
	@echo "... example/rpc_server_pb_test.s"
	@echo "... example/tcp_client_test.o"
	@echo "... example/tcp_client_test.i"
	@echo "... example/tcp_client_test.s"
	@echo "... example/tcp_server_test.o"
	@echo "... example/tcp_server_test.i"
	@echo "... example/tcp_server_test.s"
	@echo "... example/zkTest.o"
	@echo "... example/zkTest.i"
	@echo "... example/zkTest.s"
	@echo "... src/context.o"
	@echo "... src/context.i"
	@echo "... src/context.s"
	@echo "... src/coroutine.o"
	@echo "... src/coroutine.i"
	@echo "... src/coroutine.s"
	@echo "... src/epoller.o"
	@echo "... src/epoller.i"
	@echo "... src/epoller.s"
	@echo "... src/log.o"
	@echo "... src/log.i"
	@echo "... src/log.s"
	@echo "... src/mstime.o"
	@echo "... src/mstime.i"
	@echo "... src/mstime.s"
	@echo "... src/netco_api.o"
	@echo "... src/netco_api.i"
	@echo "... src/netco_api.s"
	@echo "... src/processor.o"
	@echo "... src/processor.i"
	@echo "... src/processor.s"
	@echo "... src/processor_selector.o"
	@echo "... src/processor_selector.i"
	@echo "... src/processor_selector.s"
	@echo "... src/rpc_proto/int_message.pb.o"
	@echo "... src/rpc_proto/int_message.pb.i"
	@echo "... src/rpc_proto/int_message.pb.s"
	@echo "... src/rpc_proto/load_balancer.o"
	@echo "... src/rpc_proto/load_balancer.i"
	@echo "... src/rpc_proto/load_balancer.s"
	@echo "... src/rpc_proto/locality_aware_load_balancer.o"
	@echo "... src/rpc_proto/locality_aware_load_balancer.i"
	@echo "... src/rpc_proto/locality_aware_load_balancer.s"
	@echo "... src/rpc_proto/rpc_client.o"
	@echo "... src/rpc_proto/rpc_client.i"
	@echo "... src/rpc_proto/rpc_client.s"
	@echo "... src/rpc_proto/rpc_client_stub.o"
	@echo "... src/rpc_proto/rpc_client_stub.i"
	@echo "... src/rpc_proto/rpc_client_stub.s"
	@echo "... src/rpc_proto/rpc_response_header.pb.o"
	@echo "... src/rpc_proto/rpc_response_header.pb.i"
	@echo "... src/rpc_proto/rpc_response_header.pb.s"
	@echo "... src/rpc_proto/rpc_server.o"
	@echo "... src/rpc_proto/rpc_server.i"
	@echo "... src/rpc_proto/rpc_server.s"
	@echo "... src/rpc_proto/rpc_server_stub.o"
	@echo "... src/rpc_proto/rpc_server_stub.i"
	@echo "... src/rpc_proto/rpc_server_stub.s"
	@echo "... src/rpc_proto/rpcheader.pb.o"
	@echo "... src/rpc_proto/rpcheader.pb.i"
	@echo "... src/rpc_proto/rpcheader.pb.s"
	@echo "... src/rpc_proto/zk_naming_service.o"
	@echo "... src/rpc_proto/zk_naming_service.i"
	@echo "... src/rpc_proto/zk_naming_service.s"
	@echo "... src/scheduler.o"
	@echo "... src/scheduler.i"
	@echo "... src/scheduler.s"
	@echo "... src/socket.o"
	@echo "... src/socket.i"
	@echo "... src/socket.s"
	@echo "... src/tcp/tcp_client.o"
	@echo "... src/tcp/tcp_client.i"
	@echo "... src/tcp/tcp_client.s"
	@echo "... src/tcp/tcp_server.o"
	@echo "... src/tcp/tcp_server.i"
	@echo "... src/tcp/tcp_server.s"
	@echo "... src/timer.o"
	@echo "... src/timer.i"
	@echo "... src/timer.s"
	@echo "... src/zk_client.o"
	@echo "... src/zk_client.i"
	@echo "... src/zk_client.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

