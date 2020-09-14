cmake_minimum_required(VERSION 3.17)
project(Edge_Collider)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -municode")

add_executable(Edge_Collider main.cpp ColorFlow/ColorFlow.cpp ColorFlow/ColorFlow.h)