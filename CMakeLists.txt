cmake_minimum_required(VERSION 3.16)

project(FileStore
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(FileStore
    src/filestore.cpp
)

target_include_directories(FileStore
    PUBLIC
    include
)

Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests
    test/hash.cpp
)

target_link_libraries(tests Catch2::Catch2WithMain FileStore)
