project(Emu_Intel8080_Wrapper)

if (LINUX OR UNIX)
    add_executable(Emu_Intel8080_Wrapper WrapperLinux.cpp)
elseif (WIN32)
    add_executable(Emu_Intel8080_Wrapper WrapperWindows.cpp)
endif ()