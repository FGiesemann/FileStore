cmake_minimum_required(VERSION 3.16)

project(FileStore
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(Catch2 REQUIRED)

add_library(FileStore
    src/file.cpp
    src/filestore.cpp
    src/sha256.cpp
)

target_include_directories(FileStore
    PUBLIC
    include
)

add_executable(tests
    test/file.cpp
    test/hash.cpp
    test/sha256.cpp
    test/filestore.cpp
)

target_link_libraries(tests 
    PRIVATE Catch2::Catch2WithMain FileStore
)
