cmake_minimum_required(VERSION 3.0.0)

project(CommunicationLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Network)

set(SOURCES
    MessageMediatorTcpSocket.cpp
    MessageMediator.cpp
)

set(HEADERS
    MessageMediator.h
    MessageMediatorTcpSocket.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)
