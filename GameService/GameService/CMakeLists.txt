cmake_minimum_required(VERSION 3.0.0)

project(GameService VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Core Network)

include_directories(${CommunicationLib_SOURCE_DIR})

set(SOURCES
    main.cpp
    BotClient.cpp
    BotClientCommunicator.cpp
    BotLocationReceiver.cpp
    BotTrackingService.cpp
    RemoteAccessPoint.cpp
    VisualizerAccessPoint.cpp
)

set(HEADERS
    BotClient.h
    BotClientCommunicator.h
    BotLocationReceiver.h
    BotTrackingService.h
    RemoteAccessPoint.h
    VisualizerAccessPoint.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network CommunicationLib)
