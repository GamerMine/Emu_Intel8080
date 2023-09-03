cmake_minimum_required(VERSION 3.26)
project(Emu_Intel8080)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC  ON)
set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES
        debug/redirectQDebug.cpp
        main.cpp
        hardware/Bus.cpp
        hardware/Intel8080.cpp
        hardware/Screen.cpp
        hardware/ScreenAdapter.cpp
        hardware/Speaker.cpp
        utils/WaveFileLoader.cpp
)

set(HEADER_FILES
        hardware/Bus.h
        hardware/Intel8080.h
        hardware/Screen.h
        hardware/ScreenAdapter.h
        hardware/Speaker.h
        utils/WaveFileLoader.h
)

find_package(Qt6Widgets REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(Emu_Intel8080 ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Emu_Intel8080 Qt6::Widgets openal)

add_subdirectory(tests)