cmake_minimum_required(VERSION 3.27)
project(Assignment1_Test C)

set(CMAKE_C_STANDARD 17)

add_executable(Assignment1_Test
        src/main.c
        src/HTTPRequest.c
        include/fileTools.h
        src/fileTools.c
        include/stringTools.h
        src/stringTools.c
)

target_include_directories(Assignment1_Test PRIVATE include)
