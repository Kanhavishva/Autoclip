cmake_minimum_required(VERSION 3.5)
project(autoclip)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(autoclip ${SOURCE_FILES})

set(CMAKE_BUILD_TYPE Release)
