# Emu_Intel8080
A Space Invader emulator written in C++ with the Qt framework

# Requirements
If you are using the **Windows** version you should install [OpenAL](https://www.openal.org/downloads/).

On **Linux** it should works without any additional steps.

# Installation
- Go into the release section and download the version for you operating system (the **Linux** version has been tested on *Ubuntu 23.10 with X11* window manager)
- Extract the archive and start `run.sh` if on **Linux** or `run.bat` if on **Windows**.
- Enjoy !

# Controls
All the keys are subject to change (currently assigned keys are kind of akward x) )
- Numpad Enter key *(Yes **Numpad**)* to add credits
- & key *(Good luck to find this one)* to start a one player game
- Left and Right arrows to move left or right
- Space to shoot

# Future work
Here are some features I want to add to this project :
- Key binding (reassigning keys)
- i18n (Some translations)
- Some more items in the menu bar (like About, Quit...)
- Possibility to play with a controller (mostly Dualshocks and Xbox controllers)
- Make the window resizable to resize the game

# Building
## Linux
- Install [Qt](https://www.qt.io/download-qt-installer-oss) (current used version is 6.5.2) and OpenAL (`sudo apt install libopenal-dev` on Ubuntu 23.10)
- Clone this repository : `git clone https://github.com/GamerMine/Emu_Intel8080.git`
- Access the cloned repository : `cd Emu_Intel8080`
- Create a build directory and go into it : `mkdir build_linux && cd build_linux`
- Configure the cmake project : `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt/6.5.2/gcc_64 ..`
- Build : `cmake --build . --target Emu_Intel8080`
- (Optional) Deploy : `./deploy_linux.sh`

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
- Deploy : `./deploy_windows.sh`

If you choose to deploy the project it should be under `build_windows/build`
