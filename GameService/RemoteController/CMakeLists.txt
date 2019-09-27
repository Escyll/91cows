cmake_minimum_required(VERSION 3.0.0)

project(RemoteController VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED Core Network Gui Quick Widgets)

set(SOURCES
    main.cpp
    RemoteController.cpp
)

set(HEADERS
    RemoteController.h
)

set(RESOURCES
    remoteController.qrc
)

qt5_add_resources (RCC_SOURCES ${RESOURCES})

add_executable(${PROJECT_NAME} ${RCC_SOURCES} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Gui Qt5::Quick Qt5::Widgets CommunicationLib)
