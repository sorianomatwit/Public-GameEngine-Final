# Final Project for Game Engine Development

## Purpose
This repository contains a minimal C++ project that can be used as "blank" starter code for the Game Engine Development Final Project.  The project has raylib (https://www.raylib.com/) configured for graphics and user input and Dear ImGui (https://github.com/ocornut/imgui) for debug GUI.

## Usage

The project contains two ways to build the executable:

* A `makefile` intended for use with Mac and Linux
* A preconfigured Visual Studio project intended for use with Windows

### Notes:

I have included the required static libraries for Mac, Windows, and Linux. So, once compiled, the executables should be portable.

**For Windows**, the Visual Studio `sln` file is located in the `visualstudio` directory. The projects have been properly configured to include all required libraries to create a 64-bit executable, which will be placed in the `bin` directory.

**For Mac**, I have included raylib libraries for Arm, x86, and x86_64. The makefile will automatically determine the architecture and link as appropriate. The final executable will have a `.x` extension and be located in the `bin` directory.

**For Linux**, I have included the appropriate static libraries (compiled in WSL using Ubuntu 20.04.6 LTS). The makefile will automatically link the correct libraries. Upon successful compilation, the final executable will have a `.x` extension and be located in the `bin` directory.
