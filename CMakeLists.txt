cmake_minimum_required(VERSION 3.16)

project(FileStore
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(FileStore
    src/filestore.cpp
)

target_include_directories(FileStore
    INTERFACE
    include
)
