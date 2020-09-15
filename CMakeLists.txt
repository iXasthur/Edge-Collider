cmake_minimum_required(VERSION 3.17)
project(Edge_Collider)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -municode")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgdiplus") #target_link_libraries(Edge_Collider -lgdiplus)

add_executable(Edge_Collider main.cpp ColorFlow/ColorFlow.cpp ColorFlow/ColorFlow.h MovingObject/RainbowRect.cpp MovingObject/RainbowRect.h MovingObject/SpriteNode.cpp MovingObject/SpriteNode.h MovingObject/MovingObject.cpp MovingObject/MovingObject.h)