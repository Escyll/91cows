cmake_minimum_required(VERSION 3.0.0)

project(GameService VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED Core Network Gui)

set(SOURCES
    main.cpp
    ActionItem.cpp
    BotClientSockets.cpp
    BotInfo.cpp
    BotTrackingServiceSocket.cpp
    GameRunner.cpp
    Game.cpp
    RemoteControllerSocket.cpp
    VisualizationSocket.cpp
)

set(HEADERS
    ActionItem.h
    BotClientSockets.h
    BotInfo.h
    BotTrackingServiceSocket.h
    GameRunner.h
    Game.h
    RemoteControllerSocket.h
    VisualizationSocket.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Gui CommunicationLib CollisionDetectionLib GeometryLib MazeLib)
