cmake_minimum_required(VERSION 3.18)

project(bencode-parser CXX)

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(${PROJECT_NAME} examples/example.cxx)

include_directories(
         "include"
)