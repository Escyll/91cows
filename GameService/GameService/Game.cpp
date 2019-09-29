// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Game.h"

#include <BacktrackMazeGenerator.h>
#include <MazePostProcessing.h>
#include <MazeToAsciiArt.h>
#include <QJsonArray>
#include <QDebug>

Game::Game()
{
}

Game::Game(GameOptions options)
{
    BacktrackMazeGenerator generator;
    MazePostProcessing postProcessor;
    m_maze = generator.generateMaze(options.mazeSize);
    if (options.removeDeadEnds)
    {
        postProcessor.removeDeadEndsInInnerRegionOfMaze(m_maze);
    }
    postProcessor.removeRandomWallInInnerRegionOfMaze(m_maze, options.numberOfWallRemovals);
    qDebug() << "New game created. Maze looks like:";
    MazeToAsciiArt::drawToConsole(m_maze);
    m_state = State::Waiting;
    int typeId = 0;
    for (auto type : options.numberOfActionItems.keys())
    {
        auto total = options.numberOfActionItems[type];
        for (auto number = 0; number < options.numberOfActionItems[type]; number++)
        {
            m_actionItems << ActionItem {type, QPointF(static_cast<double>(number)/(total-1), typeId/6.0)};
        }
        typeId++;
    }
}

void Game::handleTick()
{
    if (m_state == State::Running)
    {
        m_tick++;
        handleWallCollisions();
        handleActionItems();
    }
}

void Game::start()
{
    m_tick = 0;
    m_bots.clear();
    m_state = State::Running;
}

void Game::stop()
{
    m_state = State::Stopped;
}

void Game::setBotLocations(const QVector<BotInfo>& botLocations)
{
    for (auto botLocation : botLocations)
    {
        m_bots[botLocation.arucoId] = botLocation;
    }
}

QJsonArray Game::getBotJsonArray()
{
    QJsonArray botArray;
    for (auto botInfo : m_bots)
    {
        QJsonObject bot;
        bot["arucoId"] = botInfo.arucoId;
        bot["position"] = QJsonArray() << botInfo.location.x() << botInfo.location.y();
        bot["forward"] = QJsonArray() << static_cast<double>(botInfo.forward.x()) << static_cast<double>(botInfo.forward.y());
        bot["right"] = QJsonArray() << static_cast<double>(botInfo.right.x()) << static_cast<double>(botInfo.right.y());
        botArray << bot;
    }
    return botArray;
}

void Game::insertSharedGameState(QJsonObject& jsonState)
{
    jsonState["gameId"] =  1;
    jsonState["gameState"] = m_state == State::Waiting ? "waiting"
                                                       : m_state == State::Running ? "running"
                                                                                   : "stopped";
    jsonState["gameTick"] = m_tick;
    jsonState["rows"] = m_maze.getLayout().width();
    jsonState["columns"] = m_maze.getLayout().height();
    jsonState["data"] = m_maze.toJson();
    jsonState["bots"] = getBotJsonArray();
}

QJsonObject Game::getObscuredState()
{
    QJsonObject jsonState;
    insertSharedGameState(jsonState);
    return jsonState;
}

QJsonObject Game::getRevealedState()
{
    QJsonObject jsonState;
    insertSharedGameState(jsonState);
    QJsonArray actionItemsArray;
    for (auto actionItem : m_actionItems)
    {
        actionItemsArray << QJsonObject{{"type", GameOptions::actionItemTypeToString(actionItem.type)},
                                        {"x", actionItem.location.x()},
                                        {"y", actionItem.location.y()}};
    }
    jsonState["actionItems"] = actionItemsArray;
    return jsonState;
}

void Game::handleWallCollisions()
{

}

void Game::handleActionItems()
{

}
