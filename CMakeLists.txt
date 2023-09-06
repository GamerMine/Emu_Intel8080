cmake_minimum_required(VERSION 3.26)
project(Emu_Intel8080)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC  ON)
set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES
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
if (WIN32)
    find_package(OpenAL CONFIG REQUIRED)
    set(OPEN_AL_LINK OpenAL::OpenAL)
elseif (UNIX OR LINUX)
    find_package(OpenAL REQUIRED)
    set(OPEN_AL_LINK openal)
endif ()

add_executable(Emu_Intel8080 ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Emu_Intel8080 Qt6::Widgets ${OPEN_AL_LINK})

add_subdirectory(tests)
