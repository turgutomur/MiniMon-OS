cmake_minimum_required(VERSION 3.10)

project(minimon)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SOURCES "src/*.cpp" "ui/*.cpp")

add_executable(minimon ${SOURCES})
