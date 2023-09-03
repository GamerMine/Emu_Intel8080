cmake_minimum_required(VERSION 3.26)
project(Emu_Intel8080)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC  ON)
set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES
        debug/redirectQDebug.cpp
        main.cpp
        hardware/bus.cpp
        hardware/intel_8080.cpp
        hardware/screen.cpp
        hardware/screen_adapter.cpp
        hardware/speaker.cpp
        utils/WaveFileLoader.cpp
)

set(HEADER_FILES
        hardware/bus.h
        hardware/intel_8080.h
        hardware/screen.h
        hardware/screen_adapter.h
        hardware/speaker.h
        utils/WaveFileLoader.h
)

find_package(Qt6Widgets REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(Emu_Intel8080 ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Emu_Intel8080 Qt6::Widgets openal)

add_subdirectory(tests)