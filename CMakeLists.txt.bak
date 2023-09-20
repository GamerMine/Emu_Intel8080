cmake_minimum_required(VERSION 3.26)
project(Emu_Intel8080)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC  ON)
set(CMAKE_AUTOMOC ON)
set(PROJECT_VERSION 1.2.1)

set(SOURCE_FILES
        main.cpp
        hardware/Bus.cpp
        hardware/Intel8080.cpp
        hardware/Screen.cpp
        hardware/ScreenAdapter.cpp
        hardware/Speaker.cpp
        utils/WaveFileLoader.cpp
        widgets/BorderLayout.cpp
        widgets/GameConfigDialog.cpp
)

set(HEADER_FILES
        hardware/Bus.h
        hardware/Intel8080.h
        hardware/Screen.h
        hardware/ScreenAdapter.h
        hardware/Speaker.h
        utils/WaveFileLoader.h
        widgets/BorderLayout.h
        widgets/GameConfigDialog.h
)

find_package(Qt6Widgets REQUIRED)
if (WIN32)
    find_package(OpenAL CONFIG REQUIRED)
    set(OPEN_AL_LINK OpenAL::OpenAL)
elseif (UNIX OR LINUX)
    find_package(OpenAL REQUIRED)
    set(OPEN_AL_LINK openal)
endif ()
qt_standard_project_setup()

qt_add_executable(Emu_Intel8080 ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Emu_Intel8080 PRIVATE Qt6::Widgets ${OPEN_AL_LINK})
target_include_directories(Emu_Intel8080 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
if (UNIX OR LINUX)
    file(COPY scripts/deploy_linux.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else (WIN32)
    file(COPY scripts/deploy_windows.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()

add_subdirectory(wrapper)