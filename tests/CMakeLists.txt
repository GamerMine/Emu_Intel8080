cmake_minimum_required(VERSION 3.26)
project(Emu_Intel8080_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC  ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6Widgets REQUIRED)

add_executable(Emu_Intel8080_tests intel_8080.h intel_8080.cpp intel_8080_tests.cpp)

target_link_libraries(Emu_Intel8080_tests Qt6::Widgets)