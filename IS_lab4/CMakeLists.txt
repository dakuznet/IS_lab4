cmake_minimum_required(VERSION 3.10)
project(IS_lab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(IS_lab4 
    IS_lab4.cpp 
    board.cpp 
    heuristic.cpp 
    bot.cpp
)