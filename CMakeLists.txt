cmake_minimum_required(VERSION 3.21)

project(bencode-parser)

set(CMAKE_CXX_COMPILER "clang++-13")
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CXX_FLAGS "-pipe -pedantic -Wall -Wextra -Wshadow -Wdouble-promotion -Wcast-align -Wconversion -Wfloat-equal -Wunreachable-code")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined,address ${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ${CXX_FLAGS}")

# add_executable(${PROJECT_NAME} examples/example.cxx)
add_executable(${PROJECT_NAME} test.cxx)

include_directories(
         "include"
)
