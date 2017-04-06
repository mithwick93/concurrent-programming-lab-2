cmake_minimum_required(VERSION 3.6)
project(Lab2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES serial.c)

add_executable(Lab2 ${SOURCE_FILES})

target_link_libraries(Lab2 m pthread)