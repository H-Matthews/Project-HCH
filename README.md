# Supported Platforms
- Linux (Ubuntu)
- Windows

# Requirements
Here you will find all of the necessary third party tools needed to build the application
- Git
- CMake

## Windows Dependencies
- Visual Studio 2022 / 2019

## Linux Dependencies
In order to build the application, the CMake script will first pull SFML from github and build into static libraries. If you are using linux, ensure the dependencies listed below are installed on your system.

On Ubuntu, 
```
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

## Build Process
The following commands are expecting you to be in the top level of the project
### Linux
#### Command Line (Normal Way)
```
mkdir build
cd build
cmake ../
cmake --build .
```

#### Command Line (CMake Presets) RECOMMENDED
```
cmake --list-presets # Gives you a list of optional Config presets
cmake --preset debug-unix
cmake --build --list-presets # Gives you a list of optional build presets
cmake --build --preset debug-unix
```

#### VSCode CMake Integration
##### VSCode Extensions Required
1. [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
2. [CMake Tools Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

##### Building in VSCode
With the above extensions installed in VSCode, you should have the necessary CMake commands

In VSCode open the command pallete with ```Ctrl+Shift+P ``` Type CMake for a list of the CMake extension commands. Look for ```CMake:Configure``` choose the configuration setting based on your OS. Next open the command pallete again and look for ```CMake:Build``` choose the build option based on your OS.
To Run in debug look for ```Cmake:Debug``` to run without debug look for ```Cmake:Run Without Debugging```

### Windows
#### Visual Studio 2019 / 2022
```
cmake --list-presets # Gives you a list of optional config presets
cmake --preset windows-VS22
```
Creates the .sln file in the build directory