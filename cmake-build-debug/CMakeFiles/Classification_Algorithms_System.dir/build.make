# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/share/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/share/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Classification_Algorithms_System.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Classification_Algorithms_System.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Classification_Algorithms_System.dir/flags.make

CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o: ../src/Data.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Data.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Data.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Data.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o: ../src/IMA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/IMA.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/IMA.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/IMA.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o: ../src/Kernel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Kernel.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Kernel.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Kernel.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o: ../src/Perceptron.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Perceptron.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Perceptron.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Perceptron.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o: ../src/RFE.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/RFE.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/RFE.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/RFE.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o: ../src/Utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Utils.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Utils.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Utils.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o: ../src/Validation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Validation.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Validation.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Validation.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o: ../src/Visualization.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Visualization.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Visualization.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/Visualization.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o: ../src/gnuplot_i.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/gnuplot_i.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/gnuplot_i.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/gnuplot_i.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o


CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o: CMakeFiles/Classification_Algorithms_System.dir/flags.make
CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o -c "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/main.cpp"

CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/main.cpp" > CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.i

CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/src/main.cpp" -o CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.s

CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.requires

CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.provides: CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classification_Algorithms_System.dir/build.make CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.provides

CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.provides.build: CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o


# Object files for target Classification_Algorithms_System
Classification_Algorithms_System_OBJECTS = \
"CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o" \
"CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o"

# External object files for target Classification_Algorithms_System
Classification_Algorithms_System_EXTERNAL_OBJECTS =

Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/build.make
Classification_Algorithms_System: CMakeFiles/Classification_Algorithms_System.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable Classification_Algorithms_System"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Classification_Algorithms_System.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Classification_Algorithms_System.dir/build: Classification_Algorithms_System

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/build

CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/Data.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/IMA.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/Kernel.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/Perceptron.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/RFE.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/Utils.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/Validation.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/Visualization.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/gnuplot_i.cpp.o.requires
CMakeFiles/Classification_Algorithms_System.dir/requires: CMakeFiles/Classification_Algorithms_System.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/Classification_Algorithms_System.dir/requires

CMakeFiles/Classification_Algorithms_System.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Classification_Algorithms_System.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/clean

CMakeFiles/Classification_Algorithms_System.dir/depend:
	cd "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System" "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System" "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug" "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug" "/home/mateus558/Dropbox/Aprendizado de Máquinas/Classification_Algorithms_System/cmake-build-debug/CMakeFiles/Classification_Algorithms_System.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Classification_Algorithms_System.dir/depend

