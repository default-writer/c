# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/user/c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/c

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/user/c/CMakeFiles /home/user/c//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/user/c/CMakeFiles 0
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
# Target rules for targets named c-sys

# Build rule for target.
c-sys: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 c-sys
.PHONY : c-sys

# fast build rule for target.
c-sys/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/build
.PHONY : c-sys/fast

#=============================================================================
# Target rules for targets named c-vm

# Build rule for target.
c-vm: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 c-vm
.PHONY : c-vm

# fast build rule for target.
c-vm/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/build
.PHONY : c-vm/fast

#=============================================================================
# Target rules for targets named main-tests-vm1

# Build rule for target.
main-tests-vm1: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 main-tests-vm1
.PHONY : main-tests-vm1

# fast build rule for target.
main-tests-vm1/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/build
.PHONY : main-tests-vm1/fast

src/system/api/api_v1.o: src/system/api/api_v1.c.o
.PHONY : src/system/api/api_v1.o

# target to build an object file
src/system/api/api_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/api/api_v1.c.o
.PHONY : src/system/api/api_v1.c.o

src/system/api/api_v1.i: src/system/api/api_v1.c.i
.PHONY : src/system/api/api_v1.i

# target to preprocess a source file
src/system/api/api_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/api/api_v1.c.i
.PHONY : src/system/api/api_v1.c.i

src/system/api/api_v1.s: src/system/api/api_v1.c.s
.PHONY : src/system/api/api_v1.s

# target to generate assembly for a file
src/system/api/api_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/api/api_v1.c.s
.PHONY : src/system/api/api_v1.c.s

src/system/error/error_v1.o: src/system/error/error_v1.c.o
.PHONY : src/system/error/error_v1.o

# target to build an object file
src/system/error/error_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/error/error_v1.c.o
.PHONY : src/system/error/error_v1.c.o

src/system/error/error_v1.i: src/system/error/error_v1.c.i
.PHONY : src/system/error/error_v1.i

# target to preprocess a source file
src/system/error/error_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/error/error_v1.c.i
.PHONY : src/system/error/error_v1.c.i

src/system/error/error_v1.s: src/system/error/error_v1.c.s
.PHONY : src/system/error/error_v1.s

# target to generate assembly for a file
src/system/error/error_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/error/error_v1.c.s
.PHONY : src/system/error/error_v1.c.s

src/system/hashtable/hashtable_v1.o: src/system/hashtable/hashtable_v1.c.o
.PHONY : src/system/hashtable/hashtable_v1.o

# target to build an object file
src/system/hashtable/hashtable_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/hashtable/hashtable_v1.c.o
.PHONY : src/system/hashtable/hashtable_v1.c.o

src/system/hashtable/hashtable_v1.i: src/system/hashtable/hashtable_v1.c.i
.PHONY : src/system/hashtable/hashtable_v1.i

# target to preprocess a source file
src/system/hashtable/hashtable_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/hashtable/hashtable_v1.c.i
.PHONY : src/system/hashtable/hashtable_v1.c.i

src/system/hashtable/hashtable_v1.s: src/system/hashtable/hashtable_v1.c.s
.PHONY : src/system/hashtable/hashtable_v1.s

# target to generate assembly for a file
src/system/hashtable/hashtable_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/hashtable/hashtable_v1.c.s
.PHONY : src/system/hashtable/hashtable_v1.c.s

src/system/info/info_v1.o: src/system/info/info_v1.c.o
.PHONY : src/system/info/info_v1.o

# target to build an object file
src/system/info/info_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/info/info_v1.c.o
.PHONY : src/system/info/info_v1.c.o

src/system/info/info_v1.i: src/system/info/info_v1.c.i
.PHONY : src/system/info/info_v1.i

# target to preprocess a source file
src/system/info/info_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/info/info_v1.c.i
.PHONY : src/system/info/info_v1.c.i

src/system/info/info_v1.s: src/system/info/info_v1.c.s
.PHONY : src/system/info/info_v1.s

# target to generate assembly for a file
src/system/info/info_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/info/info_v1.c.s
.PHONY : src/system/info/info_v1.c.s

src/system/memory/memory_v1.o: src/system/memory/memory_v1.c.o
.PHONY : src/system/memory/memory_v1.o

# target to build an object file
src/system/memory/memory_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/memory/memory_v1.c.o
.PHONY : src/system/memory/memory_v1.c.o

src/system/memory/memory_v1.i: src/system/memory/memory_v1.c.i
.PHONY : src/system/memory/memory_v1.i

# target to preprocess a source file
src/system/memory/memory_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/memory/memory_v1.c.i
.PHONY : src/system/memory/memory_v1.c.i

src/system/memory/memory_v1.s: src/system/memory/memory_v1.c.s
.PHONY : src/system/memory/memory_v1.s

# target to generate assembly for a file
src/system/memory/memory_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/memory/memory_v1.c.s
.PHONY : src/system/memory/memory_v1.c.s

src/system/options/options_v1.o: src/system/options/options_v1.c.o
.PHONY : src/system/options/options_v1.o

# target to build an object file
src/system/options/options_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/options/options_v1.c.o
.PHONY : src/system/options/options_v1.c.o

src/system/options/options_v1.i: src/system/options/options_v1.c.i
.PHONY : src/system/options/options_v1.i

# target to preprocess a source file
src/system/options/options_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/options/options_v1.c.i
.PHONY : src/system/options/options_v1.c.i

src/system/options/options_v1.s: src/system/options/options_v1.c.s
.PHONY : src/system/options/options_v1.s

# target to generate assembly for a file
src/system/options/options_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/options/options_v1.c.s
.PHONY : src/system/options/options_v1.c.s

src/system/os/os_v1.o: src/system/os/os_v1.c.o
.PHONY : src/system/os/os_v1.o

# target to build an object file
src/system/os/os_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/os/os_v1.c.o
.PHONY : src/system/os/os_v1.c.o

src/system/os/os_v1.i: src/system/os/os_v1.c.i
.PHONY : src/system/os/os_v1.i

# target to preprocess a source file
src/system/os/os_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/os/os_v1.c.i
.PHONY : src/system/os/os_v1.c.i

src/system/os/os_v1.s: src/system/os/os_v1.c.s
.PHONY : src/system/os/os_v1.s

# target to generate assembly for a file
src/system/os/os_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-sys.dir/build.make CMakeFiles/c-sys.dir/src/system/os/os_v1.c.s
.PHONY : src/system/os/os_v1.c.s

src/virtual/api/api_v1.o: src/virtual/api/api_v1.c.o
.PHONY : src/virtual/api/api_v1.o

# target to build an object file
src/virtual/api/api_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/api/api_v1.c.o
.PHONY : src/virtual/api/api_v1.c.o

src/virtual/api/api_v1.i: src/virtual/api/api_v1.c.i
.PHONY : src/virtual/api/api_v1.i

# target to preprocess a source file
src/virtual/api/api_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/api/api_v1.c.i
.PHONY : src/virtual/api/api_v1.c.i

src/virtual/api/api_v1.s: src/virtual/api/api_v1.c.s
.PHONY : src/virtual/api/api_v1.s

# target to generate assembly for a file
src/virtual/api/api_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/api/api_v1.c.s
.PHONY : src/virtual/api/api_v1.c.s

src/virtual/env/env_v1.o: src/virtual/env/env_v1.c.o
.PHONY : src/virtual/env/env_v1.o

# target to build an object file
src/virtual/env/env_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/env/env_v1.c.o
.PHONY : src/virtual/env/env_v1.c.o

src/virtual/env/env_v1.i: src/virtual/env/env_v1.c.i
.PHONY : src/virtual/env/env_v1.i

# target to preprocess a source file
src/virtual/env/env_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/env/env_v1.c.i
.PHONY : src/virtual/env/env_v1.c.i

src/virtual/env/env_v1.s: src/virtual/env/env_v1.c.s
.PHONY : src/virtual/env/env_v1.s

# target to generate assembly for a file
src/virtual/env/env_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/env/env_v1.c.s
.PHONY : src/virtual/env/env_v1.c.s

src/virtual/list/list_v1.o: src/virtual/list/list_v1.c.o
.PHONY : src/virtual/list/list_v1.o

# target to build an object file
src/virtual/list/list_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/list/list_v1.c.o
.PHONY : src/virtual/list/list_v1.c.o

src/virtual/list/list_v1.i: src/virtual/list/list_v1.c.i
.PHONY : src/virtual/list/list_v1.i

# target to preprocess a source file
src/virtual/list/list_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/list/list_v1.c.i
.PHONY : src/virtual/list/list_v1.c.i

src/virtual/list/list_v1.s: src/virtual/list/list_v1.c.s
.PHONY : src/virtual/list/list_v1.s

# target to generate assembly for a file
src/virtual/list/list_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/list/list_v1.c.s
.PHONY : src/virtual/list/list_v1.c.s

src/virtual/pointer/pointer_v1.o: src/virtual/pointer/pointer_v1.c.o
.PHONY : src/virtual/pointer/pointer_v1.o

# target to build an object file
src/virtual/pointer/pointer_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/pointer/pointer_v1.c.o
.PHONY : src/virtual/pointer/pointer_v1.c.o

src/virtual/pointer/pointer_v1.i: src/virtual/pointer/pointer_v1.c.i
.PHONY : src/virtual/pointer/pointer_v1.i

# target to preprocess a source file
src/virtual/pointer/pointer_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/pointer/pointer_v1.c.i
.PHONY : src/virtual/pointer/pointer_v1.c.i

src/virtual/pointer/pointer_v1.s: src/virtual/pointer/pointer_v1.c.s
.PHONY : src/virtual/pointer/pointer_v1.s

# target to generate assembly for a file
src/virtual/pointer/pointer_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/pointer/pointer_v1.c.s
.PHONY : src/virtual/pointer/pointer_v1.c.s

src/virtual/types/data/data_v1.o: src/virtual/types/data/data_v1.c.o
.PHONY : src/virtual/types/data/data_v1.o

# target to build an object file
src/virtual/types/data/data_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/data/data_v1.c.o
.PHONY : src/virtual/types/data/data_v1.c.o

src/virtual/types/data/data_v1.i: src/virtual/types/data/data_v1.c.i
.PHONY : src/virtual/types/data/data_v1.i

# target to preprocess a source file
src/virtual/types/data/data_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/data/data_v1.c.i
.PHONY : src/virtual/types/data/data_v1.c.i

src/virtual/types/data/data_v1.s: src/virtual/types/data/data_v1.c.s
.PHONY : src/virtual/types/data/data_v1.s

# target to generate assembly for a file
src/virtual/types/data/data_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/data/data_v1.c.s
.PHONY : src/virtual/types/data/data_v1.c.s

src/virtual/types/file/file_v1.o: src/virtual/types/file/file_v1.c.o
.PHONY : src/virtual/types/file/file_v1.o

# target to build an object file
src/virtual/types/file/file_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/file/file_v1.c.o
.PHONY : src/virtual/types/file/file_v1.c.o

src/virtual/types/file/file_v1.i: src/virtual/types/file/file_v1.c.i
.PHONY : src/virtual/types/file/file_v1.i

# target to preprocess a source file
src/virtual/types/file/file_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/file/file_v1.c.i
.PHONY : src/virtual/types/file/file_v1.c.i

src/virtual/types/file/file_v1.s: src/virtual/types/file/file_v1.c.s
.PHONY : src/virtual/types/file/file_v1.s

# target to generate assembly for a file
src/virtual/types/file/file_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/file/file_v1.c.s
.PHONY : src/virtual/types/file/file_v1.c.s

src/virtual/types/object/object_v1.o: src/virtual/types/object/object_v1.c.o
.PHONY : src/virtual/types/object/object_v1.o

# target to build an object file
src/virtual/types/object/object_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/object/object_v1.c.o
.PHONY : src/virtual/types/object/object_v1.c.o

src/virtual/types/object/object_v1.i: src/virtual/types/object/object_v1.c.i
.PHONY : src/virtual/types/object/object_v1.i

# target to preprocess a source file
src/virtual/types/object/object_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/object/object_v1.c.i
.PHONY : src/virtual/types/object/object_v1.c.i

src/virtual/types/object/object_v1.s: src/virtual/types/object/object_v1.c.s
.PHONY : src/virtual/types/object/object_v1.s

# target to generate assembly for a file
src/virtual/types/object/object_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/object/object_v1.c.s
.PHONY : src/virtual/types/object/object_v1.c.s

src/virtual/types/stack/stack_v1.o: src/virtual/types/stack/stack_v1.c.o
.PHONY : src/virtual/types/stack/stack_v1.o

# target to build an object file
src/virtual/types/stack/stack_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/stack/stack_v1.c.o
.PHONY : src/virtual/types/stack/stack_v1.c.o

src/virtual/types/stack/stack_v1.i: src/virtual/types/stack/stack_v1.c.i
.PHONY : src/virtual/types/stack/stack_v1.i

# target to preprocess a source file
src/virtual/types/stack/stack_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/stack/stack_v1.c.i
.PHONY : src/virtual/types/stack/stack_v1.c.i

src/virtual/types/stack/stack_v1.s: src/virtual/types/stack/stack_v1.c.s
.PHONY : src/virtual/types/stack/stack_v1.s

# target to generate assembly for a file
src/virtual/types/stack/stack_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/stack/stack_v1.c.s
.PHONY : src/virtual/types/stack/stack_v1.c.s

src/virtual/types/string/string_v1.o: src/virtual/types/string/string_v1.c.o
.PHONY : src/virtual/types/string/string_v1.o

# target to build an object file
src/virtual/types/string/string_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/string/string_v1.c.o
.PHONY : src/virtual/types/string/string_v1.c.o

src/virtual/types/string/string_v1.i: src/virtual/types/string/string_v1.c.i
.PHONY : src/virtual/types/string/string_v1.i

# target to preprocess a source file
src/virtual/types/string/string_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/string/string_v1.c.i
.PHONY : src/virtual/types/string/string_v1.c.i

src/virtual/types/string/string_v1.s: src/virtual/types/string/string_v1.c.s
.PHONY : src/virtual/types/string/string_v1.s

# target to generate assembly for a file
src/virtual/types/string/string_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/string/string_v1.c.s
.PHONY : src/virtual/types/string/string_v1.c.s

src/virtual/types/user/user_v1.o: src/virtual/types/user/user_v1.c.o
.PHONY : src/virtual/types/user/user_v1.o

# target to build an object file
src/virtual/types/user/user_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/user/user_v1.c.o
.PHONY : src/virtual/types/user/user_v1.c.o

src/virtual/types/user/user_v1.i: src/virtual/types/user/user_v1.c.i
.PHONY : src/virtual/types/user/user_v1.i

# target to preprocess a source file
src/virtual/types/user/user_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/user/user_v1.c.i
.PHONY : src/virtual/types/user/user_v1.c.i

src/virtual/types/user/user_v1.s: src/virtual/types/user/user_v1.c.s
.PHONY : src/virtual/types/user/user_v1.s

# target to generate assembly for a file
src/virtual/types/user/user_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/types/user/user_v1.c.s
.PHONY : src/virtual/types/user/user_v1.c.s

src/virtual/virtual/virtual_v1.o: src/virtual/virtual/virtual_v1.c.o
.PHONY : src/virtual/virtual/virtual_v1.o

# target to build an object file
src/virtual/virtual/virtual_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/virtual/virtual_v1.c.o
.PHONY : src/virtual/virtual/virtual_v1.c.o

src/virtual/virtual/virtual_v1.i: src/virtual/virtual/virtual_v1.c.i
.PHONY : src/virtual/virtual/virtual_v1.i

# target to preprocess a source file
src/virtual/virtual/virtual_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/virtual/virtual_v1.c.i
.PHONY : src/virtual/virtual/virtual_v1.c.i

src/virtual/virtual/virtual_v1.s: src/virtual/virtual/virtual_v1.c.s
.PHONY : src/virtual/virtual/virtual_v1.s

# target to generate assembly for a file
src/virtual/virtual/virtual_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/virtual/virtual_v1.c.s
.PHONY : src/virtual/virtual/virtual_v1.c.s

src/virtual/vm/vm_v1.o: src/virtual/vm/vm_v1.c.o
.PHONY : src/virtual/vm/vm_v1.o

# target to build an object file
src/virtual/vm/vm_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/vm/vm_v1.c.o
.PHONY : src/virtual/vm/vm_v1.c.o

src/virtual/vm/vm_v1.i: src/virtual/vm/vm_v1.c.i
.PHONY : src/virtual/vm/vm_v1.i

# target to preprocess a source file
src/virtual/vm/vm_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/vm/vm_v1.c.i
.PHONY : src/virtual/vm/vm_v1.c.i

src/virtual/vm/vm_v1.s: src/virtual/vm/vm_v1.c.s
.PHONY : src/virtual/vm/vm_v1.s

# target to generate assembly for a file
src/virtual/vm/vm_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/c-vm.dir/build.make CMakeFiles/c-vm.dir/src/virtual/vm/vm_v1.c.s
.PHONY : src/virtual/vm/vm_v1.c.s

tests/vm/main-tests_v1.o: tests/vm/main-tests_v1.c.o
.PHONY : tests/vm/main-tests_v1.o

# target to build an object file
tests/vm/main-tests_v1.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/main-tests_v1.c.o
.PHONY : tests/vm/main-tests_v1.c.o

tests/vm/main-tests_v1.i: tests/vm/main-tests_v1.c.i
.PHONY : tests/vm/main-tests_v1.i

# target to preprocess a source file
tests/vm/main-tests_v1.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/main-tests_v1.c.i
.PHONY : tests/vm/main-tests_v1.c.i

tests/vm/main-tests_v1.s: tests/vm/main-tests_v1.c.s
.PHONY : tests/vm/main-tests_v1.s

# target to generate assembly for a file
tests/vm/main-tests_v1.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/main-tests_v1.c.s
.PHONY : tests/vm/main-tests_v1.c.s

tests/vm/test_api.o: tests/vm/test_api.c.o
.PHONY : tests/vm/test_api.o

# target to build an object file
tests/vm/test_api.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_api.c.o
.PHONY : tests/vm/test_api.c.o

tests/vm/test_api.i: tests/vm/test_api.c.i
.PHONY : tests/vm/test_api.i

# target to preprocess a source file
tests/vm/test_api.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_api.c.i
.PHONY : tests/vm/test_api.c.i

tests/vm/test_api.s: tests/vm/test_api.c.s
.PHONY : tests/vm/test_api.s

# target to generate assembly for a file
tests/vm/test_api.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_api.c.s
.PHONY : tests/vm/test_api.c.s

tests/vm/test_hashtable.o: tests/vm/test_hashtable.c.o
.PHONY : tests/vm/test_hashtable.o

# target to build an object file
tests/vm/test_hashtable.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_hashtable.c.o
.PHONY : tests/vm/test_hashtable.c.o

tests/vm/test_hashtable.i: tests/vm/test_hashtable.c.i
.PHONY : tests/vm/test_hashtable.i

# target to preprocess a source file
tests/vm/test_hashtable.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_hashtable.c.i
.PHONY : tests/vm/test_hashtable.c.i

tests/vm/test_hashtable.s: tests/vm/test_hashtable.c.s
.PHONY : tests/vm/test_hashtable.s

# target to generate assembly for a file
tests/vm/test_hashtable.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_hashtable.c.s
.PHONY : tests/vm/test_hashtable.c.s

tests/vm/test_list.o: tests/vm/test_list.c.o
.PHONY : tests/vm/test_list.o

# target to build an object file
tests/vm/test_list.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_list.c.o
.PHONY : tests/vm/test_list.c.o

tests/vm/test_list.i: tests/vm/test_list.c.i
.PHONY : tests/vm/test_list.i

# target to preprocess a source file
tests/vm/test_list.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_list.c.i
.PHONY : tests/vm/test_list.c.i

tests/vm/test_list.s: tests/vm/test_list.c.s
.PHONY : tests/vm/test_list.s

# target to generate assembly for a file
tests/vm/test_list.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_list.c.s
.PHONY : tests/vm/test_list.c.s

tests/vm/test_memory.o: tests/vm/test_memory.c.o
.PHONY : tests/vm/test_memory.o

# target to build an object file
tests/vm/test_memory.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_memory.c.o
.PHONY : tests/vm/test_memory.c.o

tests/vm/test_memory.i: tests/vm/test_memory.c.i
.PHONY : tests/vm/test_memory.i

# target to preprocess a source file
tests/vm/test_memory.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_memory.c.i
.PHONY : tests/vm/test_memory.c.i

tests/vm/test_memory.s: tests/vm/test_memory.c.s
.PHONY : tests/vm/test_memory.s

# target to generate assembly for a file
tests/vm/test_memory.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_memory.c.s
.PHONY : tests/vm/test_memory.c.s

tests/vm/test_pointer.o: tests/vm/test_pointer.c.o
.PHONY : tests/vm/test_pointer.o

# target to build an object file
tests/vm/test_pointer.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_pointer.c.o
.PHONY : tests/vm/test_pointer.c.o

tests/vm/test_pointer.i: tests/vm/test_pointer.c.i
.PHONY : tests/vm/test_pointer.i

# target to preprocess a source file
tests/vm/test_pointer.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_pointer.c.i
.PHONY : tests/vm/test_pointer.c.i

tests/vm/test_pointer.s: tests/vm/test_pointer.c.s
.PHONY : tests/vm/test_pointer.s

# target to generate assembly for a file
tests/vm/test_pointer.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_pointer.c.s
.PHONY : tests/vm/test_pointer.c.s

tests/vm/test_vm.o: tests/vm/test_vm.c.o
.PHONY : tests/vm/test_vm.o

# target to build an object file
tests/vm/test_vm.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_vm.c.o
.PHONY : tests/vm/test_vm.c.o

tests/vm/test_vm.i: tests/vm/test_vm.c.i
.PHONY : tests/vm/test_vm.i

# target to preprocess a source file
tests/vm/test_vm.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_vm.c.i
.PHONY : tests/vm/test_vm.c.i

tests/vm/test_vm.s: tests/vm/test_vm.c.s
.PHONY : tests/vm/test_vm.s

# target to generate assembly for a file
tests/vm/test_vm.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/main-tests-vm1.dir/build.make CMakeFiles/main-tests-vm1.dir/tests/vm/test_vm.c.s
.PHONY : tests/vm/test_vm.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... c-sys"
	@echo "... c-vm"
	@echo "... main-tests-vm1"
	@echo "... src/system/api/api_v1.o"
	@echo "... src/system/api/api_v1.i"
	@echo "... src/system/api/api_v1.s"
	@echo "... src/system/error/error_v1.o"
	@echo "... src/system/error/error_v1.i"
	@echo "... src/system/error/error_v1.s"
	@echo "... src/system/hashtable/hashtable_v1.o"
	@echo "... src/system/hashtable/hashtable_v1.i"
	@echo "... src/system/hashtable/hashtable_v1.s"
	@echo "... src/system/info/info_v1.o"
	@echo "... src/system/info/info_v1.i"
	@echo "... src/system/info/info_v1.s"
	@echo "... src/system/memory/memory_v1.o"
	@echo "... src/system/memory/memory_v1.i"
	@echo "... src/system/memory/memory_v1.s"
	@echo "... src/system/options/options_v1.o"
	@echo "... src/system/options/options_v1.i"
	@echo "... src/system/options/options_v1.s"
	@echo "... src/system/os/os_v1.o"
	@echo "... src/system/os/os_v1.i"
	@echo "... src/system/os/os_v1.s"
	@echo "... src/virtual/api/api_v1.o"
	@echo "... src/virtual/api/api_v1.i"
	@echo "... src/virtual/api/api_v1.s"
	@echo "... src/virtual/env/env_v1.o"
	@echo "... src/virtual/env/env_v1.i"
	@echo "... src/virtual/env/env_v1.s"
	@echo "... src/virtual/list/list_v1.o"
	@echo "... src/virtual/list/list_v1.i"
	@echo "... src/virtual/list/list_v1.s"
	@echo "... src/virtual/pointer/pointer_v1.o"
	@echo "... src/virtual/pointer/pointer_v1.i"
	@echo "... src/virtual/pointer/pointer_v1.s"
	@echo "... src/virtual/types/data/data_v1.o"
	@echo "... src/virtual/types/data/data_v1.i"
	@echo "... src/virtual/types/data/data_v1.s"
	@echo "... src/virtual/types/file/file_v1.o"
	@echo "... src/virtual/types/file/file_v1.i"
	@echo "... src/virtual/types/file/file_v1.s"
	@echo "... src/virtual/types/object/object_v1.o"
	@echo "... src/virtual/types/object/object_v1.i"
	@echo "... src/virtual/types/object/object_v1.s"
	@echo "... src/virtual/types/stack/stack_v1.o"
	@echo "... src/virtual/types/stack/stack_v1.i"
	@echo "... src/virtual/types/stack/stack_v1.s"
	@echo "... src/virtual/types/string/string_v1.o"
	@echo "... src/virtual/types/string/string_v1.i"
	@echo "... src/virtual/types/string/string_v1.s"
	@echo "... src/virtual/types/user/user_v1.o"
	@echo "... src/virtual/types/user/user_v1.i"
	@echo "... src/virtual/types/user/user_v1.s"
	@echo "... src/virtual/virtual/virtual_v1.o"
	@echo "... src/virtual/virtual/virtual_v1.i"
	@echo "... src/virtual/virtual/virtual_v1.s"
	@echo "... src/virtual/vm/vm_v1.o"
	@echo "... src/virtual/vm/vm_v1.i"
	@echo "... src/virtual/vm/vm_v1.s"
	@echo "... tests/vm/main-tests_v1.o"
	@echo "... tests/vm/main-tests_v1.i"
	@echo "... tests/vm/main-tests_v1.s"
	@echo "... tests/vm/test_api.o"
	@echo "... tests/vm/test_api.i"
	@echo "... tests/vm/test_api.s"
	@echo "... tests/vm/test_hashtable.o"
	@echo "... tests/vm/test_hashtable.i"
	@echo "... tests/vm/test_hashtable.s"
	@echo "... tests/vm/test_list.o"
	@echo "... tests/vm/test_list.i"
	@echo "... tests/vm/test_list.s"
	@echo "... tests/vm/test_memory.o"
	@echo "... tests/vm/test_memory.i"
	@echo "... tests/vm/test_memory.s"
	@echo "... tests/vm/test_pointer.o"
	@echo "... tests/vm/test_pointer.i"
	@echo "... tests/vm/test_pointer.s"
	@echo "... tests/vm/test_vm.o"
	@echo "... tests/vm/test_vm.i"
	@echo "... tests/vm/test_vm.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

