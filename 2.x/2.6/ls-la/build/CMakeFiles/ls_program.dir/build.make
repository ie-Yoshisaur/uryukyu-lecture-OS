# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.25.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.25.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build

# Include any dependencies generated for this target.
include CMakeFiles/ls_program.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ls_program.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ls_program.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ls_program.dir/flags.make

CMakeFiles/ls_program.dir/main.c.o: CMakeFiles/ls_program.dir/flags.make
CMakeFiles/ls_program.dir/main.c.o: /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/main.c
CMakeFiles/ls_program.dir/main.c.o: CMakeFiles/ls_program.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ls_program.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ls_program.dir/main.c.o -MF CMakeFiles/ls_program.dir/main.c.o.d -o CMakeFiles/ls_program.dir/main.c.o -c /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/main.c

CMakeFiles/ls_program.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ls_program.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/main.c > CMakeFiles/ls_program.dir/main.c.i

CMakeFiles/ls_program.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ls_program.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/main.c -o CMakeFiles/ls_program.dir/main.c.s

# Object files for target ls_program
ls_program_OBJECTS = \
"CMakeFiles/ls_program.dir/main.c.o"

# External object files for target ls_program
ls_program_EXTERNAL_OBJECTS =

ls_program: CMakeFiles/ls_program.dir/main.c.o
ls_program: CMakeFiles/ls_program.dir/build.make
ls_program: CMakeFiles/ls_program.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ls_program"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ls_program.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ls_program.dir/build: ls_program
.PHONY : CMakeFiles/ls_program.dir/build

CMakeFiles/ls_program.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ls_program.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ls_program.dir/clean

CMakeFiles/ls_program.dir/depend:
	cd /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.6/ls-la/build/CMakeFiles/ls_program.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ls_program.dir/depend
