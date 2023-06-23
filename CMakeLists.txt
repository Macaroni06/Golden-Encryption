cmake_minimum_required(VERSION 3.24)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_EXE_LINKER_FLAGS  "-static")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc_s_seh -static-libgcc")
set(SOURCE_FILES main.cpp)
add_executable(untitled1 main.cpp)
