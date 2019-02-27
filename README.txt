I have written a simple CMakeList file to speed up the process of compilation. 
If you want to skip CMake you can simply create a VS/XCode project/solution and manually add files. 
Files to be added if you are not using CMake: main.cpp SA_cache.cpp SA_cache.h

How to compile Short Version:
	Use CMake to configure then build. 

How to compile Long Version: 
	IDE and GUI compile:
		On Windows/OS X: 
		Download CMake GUI on the official website https://cmake.org/
		Open CMake, choose this directory as the Source Path, choose another path as the build path, then click "Configure".
		Select the generator option you want (Visual Studio/XCode ect. ). Some red boxes should appear, click "Configure" again and then click "Generate". Open the generrated Project/Solution.
	
	Command line compile: (Recommended)
		On Linux: download and install CMake on common Linux software installing tools like apt/dnf/yum etc. 
		Inside this directory, execute
		$ cmake ./
		$ make
		The code is then compiled. To execute: 
		$ ./simulate

By default, the program will read "test.trace" then output the result to "test.result". 
If two command line arguments are specified, the program will read the first argument as input file and write the result to a file specified by the second argument. 