cmake_minimum_required(VERSION 3.0.0)

project(MazeLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core)

set(SOURCES
    BacktrackMazeGenerator.cpp
    MazePostProcessing.cpp
    MazeToAsciiArt.cpp
    Maze.cpp
    RandomMazeGenerator.cpp
)

set(HEADERS
    BacktrackMazeGenerator.h
    MazeGenerator.h
    MazePostProcessing.h
    MazeToAsciiArt.h
    Maze.h
    RandomMazeGenerator.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} Qt5::Core)
