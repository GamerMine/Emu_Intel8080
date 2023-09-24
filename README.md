# Emu_Intel8080
A Space Invader emulator written in C++ with the Qt framework

# Requirements
If you are using the **Windows** version you should install [OpenAL](https://www.openal.org/downloads/).

On **Linux** it should works without any additional steps.

# Installation
- Go into the release section and download the version for you operating system (the **Linux** version has been tested on *Ubuntu 23.10 with X11* window manager)
- Extract the archive and start `SpaceInvader` if on **Linux** or `SpaceInvader.exe` if on **Windows**.
- Enjoy !

# Controls
All the keys are subject to change (currently assigned keys are kind of akward x) )
- Numpad Enter key *(Yes **Numpad**)* to add credits
- & key *(Good luck to find this one)* to start a one player game
- Left and Right arrows to move left or right
- Space to shoot

# Future work
Here are some features I want to add to this project :
- :ballot_box_with_check: Key binding (reassigning keys)
- :white_large_square: i18n (Some translations)
- :white_large_square: Some more items in the menu bar (like About, Quit...)
- :white_large_square: Possibility to play with a controller (mostly Dualshocks and Xbox controllers)
- :white_check_mark: Make the window resizable to resize the game

:white_large_square: = To Do
:ballot_box_with_check: = Done but not included in the latest builds (Not released yet)
:white_check_mark: = Done and included in the latest builds (Released)

# Building
## Dependencies
- [Qt](https://www.qt.io/download-qt-installer-oss) (>=6.5.2)
- [OpenAL](https://github.com/kcat/openal-soft) (>=1.23.1)
  
## Linux
- Clone this repository : `git clone https://github.com/GamerMine/Emu_Intel8080.git`
- Access the cloned repository : `cd Emu_Intel8080`
- Create a build directory and go into it : `mkdir build_linux && cd build_linux`
- Configure the cmake project : `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt/6.5.2/gcc_64 ..`
- Build : `cmake --build . --target Emu_Intel8080`
- (Optional) Deploy : `./deploy_linux.sh` (Some variables inside the script sould be modified depending on your installation)

If you choose to deploy the project it should be under `build_linux/build`

## Windows (Cross-compiling from Linux)
For this we need to compile Qt from sources to get the mingw version of Qt

- Install MinGW (`sudo apt install mingw-w64` on Ubuntu 23.10)
- Get the [Qt sources](https://download.qt.io/archive/qt/)
- Extract the tar archive with `tar -xvf qt-everywhere-src-6.5.2.tar.xz`
- Access the extracted archive : `cd qt-everywhere-src-6.5.2`
- Configure the cmake project : `./configure -prefix /path/to/Qt/6.5.2/mingw-w64 -platform linux-g++ -xplatform win32-g++ -device-option CROSS_COMPILE=/usr/bin/x86_64-w64-mingw32- -opensource -opengl desktop -qt-host-path /path/to/Qt/6.5.2/gcc_64 -nomake tests -nomake examples -skip qtwebengine -skip qttools -skip qtdoc -skip qtmultimedia -skip qtquick3dphysics -- -DQT_BUILD_TOOLS_WHEN_CROSSCOMPILING=ON -DCMAKE_TOOLCHAIN_FILE=/path/to/WindowsToolchain.cmake` (WindowsToolchain.cmake is available in the root of this project, also, the path where you extracted teh qt sources should not not contain any special characters otherwise the build will fail!)
- Build : `cmake --build .`
- Install the library : `sudo cmake --install .`

You should also compile OpenAL manually

- Clone the openal-soft on Github : `git clone https://github.com/kcat/openal-soft.git`
- Access the cloned repository/build : `cd openal-soft/buid`
- Configure the build : `cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/WindowsToolchain.cmake -DALSOFT_BACKEND_PIPEWIRE=FALSE` (I forcibly disable Pipewire build for a windows build, you should have at least the DirectSound backend detected. If other backend are making the compiling crash try to disable them and try again. WindowsToolchain.cmake is available in the root of this project.)
- Build : `cmake --build .`
- Install the library : `cmake --install .`

Now you can build this project :

- Clone this repository : `git clone https://github.com/GamerMine/Emu_Intel8080.git`
- Access the cloned repository : `cd Emu_Intel8080`
- Create a build directory and go into it : `mkdir build_windows && cd build_windows`
- Configure the cmake project : `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/x86_64-w64-mingw32-gcc-12-posix -DCMAKE_CXX_COMPILER=/usr/bin/x86_64-w64-mingw32-c++-posix -DCMAKE_PREFIX_PATH=/path/to/Qt/6.5.2/mingw-w64 -DCMAKE_TOOLCHAIN_FILE=/path/to/WindowsToolchain.cmake ..`
- Build : `cmake --build . --target Emu_Intel8080`
- Deploy : `./deploy_windows.sh` (Some variables inside the script sould be modified depending on your installation)

If you choose to deploy the project it should be under `build_windows/build`

## Windows (Using MSYS2, currently WIP)
You must have installed [MSYS2](https://www.msys2.org/) on your system before continuing.
The following instructions uses the ninja build tool, you can use other build tools by modifing the following instructions.

- Clone this repository : `git clone https://github.com/GamerMine/Emu_Intel8080.git`
- Access the cloned repository : `cd Emu_Intel8080`
- Create a build directory and access it : `mkdir build_windows && cd build_windows`
- Configure the cmake project : `cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_PREFIX_PATH="C:\Qt\6.5.2\mingw_64" -G "Ninja"`
- Build : `cmake --build .`

# References
- [Space Invaders english Wikipedia page](https://en.wikipedia.org/wiki/Space_Invaders)
- [Space Invaders Hardware map on walkofmind.com](https://walkofmind.com/programming/side/hardware.htm)
- [Space Invaders Arcade Hardware on computerarcheology.com](https://www.computerarcheology.com/Arcade/SpaceInvaders/)
- [Intel 8080 OPCODES on pastraiser.com](https://pastraiser.com/cpu/i8080/i8080_opcodes.html)
- [Intel 8080/8085 opcode table](https://tobiasvl.github.io/optable/intel-8080/)
- [Intel 8080 Assembly Language Programming Manual](https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf)
