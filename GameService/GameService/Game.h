// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "GameOptions.h"

#include <Maze.h>
#include <QJsonObject>
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

public:
    QJsonObject getObscuredState();
    QJsonObject getRevealedState();

private:
    int m_tick = 0;
    Maze m_maze;
    QVector2D m_botForward = QVector2D(0, 0.025f);
    QVector2D m_botRight = QVector2D(0.025f, 0);
    QPointF m_botLocation = QPointF(0.5, 0.5);
};

#endif // GAME_H
