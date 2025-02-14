# Supported Platforms
- Linux (Ubuntu)
- Windows

# Wiki
For more insight into the project, see the [wiki](https://github.com/H-Matthews/Project-HCH/wiki)

# Requirements
Here you will find all of the necessary third party tools needed to build the application
- Git
- CMake (at least 3.30)

## Windows Dependencies
- Visual Studio 2022 / 2019

## Linux Dependencies
In order to build the application, the CMake script will first pull SFML from github and build into static libraries. If you are using linux, ensure the dependencies listed below are installed on your system.

On Ubuntu, 
```
sudo apt update sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libflac-dev \
    libvorbis-dev \ 
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev
```

## Build Process
The following commands are expecting you to be in the top level of the project
### Linux
Best way to build without issues are to use the configured presets that already exist.

Presets:
1. debug-unix_makefiles
    - Debug build using Unix Makefiles Generator
2. release-unix_makefiles
    - Release build using Unix Makefiles Generator
3. windows-VS22
    - Build using Visual Studio 2022 multi generator
4. windows-VS19
    - Build using Visual Studio 2019 multi generator

#### Command Line (CMake Presets) RECOMMENDED
```
cmake --list-presets # Gives you a list of optional Config presets
cmake --preset debug-unix_makefiles
cmake --build --list-presets # Gives you a list of optional build presets
cmake --build --preset debug-unix_makefiles
```

#### VSCode CMake Integration
##### VSCode Extensions Required
1. [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
2. [CMake Tools Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

##### Building in VSCode
With the above extensions installed in VSCode, you should have the necessary CMake commands. 

NOTE: When building in VSCode, you are using the presets that reside in CMakePresets.json

In VSCode open the command pallete with ```Ctrl+Shift+P ``` Type CMake for a list of the CMake extension commands. Look for ```CMake:Select Configure Preset``` choose the configuration setting you would like to build (Debug | Release). Next open the command pallete again and look for ```CMake:Build``` This will build the project and create the executable.
When running, you need to make sure you select the correct launch configuration. In the RUN AND DEBUG panel, If you configured with the Release option, ensure you are running with the Release launch config. Then, select the Play button in the panel, OR press 'F5'.

### Windows
#### Visual Studio 2019 / 2022
```
cmake --list-presets # Gives you a list of optional config presets
cmake --preset windows-VS22
```
Creates the .sln file in the build directory

## Running Unit Tests
The GTest binaries get output to the ./bin/tests
To run a binary, simply navigate to that directory and execute it. 

Command to run ALL GTests
```
ctest --test-dir ./build --output-on-failure
```