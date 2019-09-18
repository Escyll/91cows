// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Game.h"

#include <BacktrackMazeGenerator.h>
#include <MazePostProcessing.h>
#include <MazeToAsciiArt.h>
#include <QJsonArray>

Game::Game()
    : m_maze(BacktrackMazeGenerator().generateMaze(QSize(10, 10)))
{
}

Game::Game(GameOptions options)
    : m_maze(BacktrackMazeGenerator().generateMaze(options.mazeSize))
{
}

void Game::handleTick()
{
    m_tick++;
}

void Game::start()
{
    BacktrackMazeGenerator generator;
    MazePostProcessing postProcessor;
    m_maze = generator.generateMaze(QSize(10, 10));
    postProcessor.removeDeadEndsInInnerRegionOfMaze(m_maze);
    postProcessor.removeRandomWallInInnerRegionOfMaze(m_maze, 20);
    MazeToAsciiArt::drawToConsole(m_maze);
    m_tick = 0;
}

void Game::stop()
{
}

QJsonObject Game::getRevealedState()
{
    QJsonObject jsonState;
    jsonState["gameId"] =  1;
    jsonState["gameTick"] = m_tick;
    jsonState["rows"] = m_maze.getLayout().width();
    jsonState["columns"] = m_maze.getLayout().height();
    jsonState["data"] = m_maze.toJson();
    QJsonArray botArray;
    QJsonObject bot;
    bot["arucoId"] = 1;
    bot["position"] = QJsonArray() << m_botLocation.x() << m_botLocation.y();
    bot["forward"] = QJsonArray() << m_botForward.x() << m_botForward.y();
    bot["right"] = QJsonArray() << m_botRight.x() << m_botRight.y();
    botArray << bot;
    jsonState["bots"] = botArray;
    return jsonState;
}
