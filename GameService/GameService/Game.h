// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "ActionItem.h"
#include "BotInfo.h"
#include "GameOptions.h"

#include <Maze.h>
#include <QJsonObject>
#include <QMap>
#include <QVector2D>

#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game();
    Game(GameOptions options);
    enum class State {
        Waiting,
        Running,
        Stopped
    };
    void handleTick();
    void start();
    void stop();
    void setBotLocations(const QVector<BotInfo>& botLocations);

public:
    QJsonObject getObscuredState();
    QJsonObject getRevealedState();

private:
    void handleWallCollisions();
    void handleCollectables();
    QJsonArray getBotJsonArray();
    void insertSharedGameState(QJsonObject& jsonState);

private:
    State m_state = State::Stopped;
    int m_tick = 0;
    Maze m_maze;
    QMap<int, BotInfo> m_bots;
    QVector<ActionItem> m_actionItems;
};

#endif // GAME_H
