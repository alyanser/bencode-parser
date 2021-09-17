cmake_minimum_required(VERSION 3.21)

project(bencode_parser)

set(CMAKE_CXX_COMPILER "clang++-13")
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CXX_FLAGS "-pipe -pedantic -Wall -Wextra -Wshadow -Wdouble-promotion -Wcast-align -Wconversion -Wfloat-equal -Wunreachable-code")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -fno-omit-frame-pointer ${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ${CXX_FLAGS}")

set(SOURCES
	tests/main.cxx
)

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(
         "include"
)