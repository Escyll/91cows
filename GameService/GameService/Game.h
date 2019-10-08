// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "ActionItem.h"
#include "BotInfo.h"
#include "GameOptions.h"
#include <CollisionDetector.h>

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
    void handleActionItems();
    QJsonArray getBotJsonArray();
    void insertSharedGameState(QJsonObject& jsonState);
    void placeActionItem(GameOptions::ActionItemType type);

private:
    void handleCollisions();
    void handleCollisionPerBotInfo(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, BotInfo& botInfo);
    bool hasWallCollision(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments);
    QVector<LineSegment> getBotLineSegments(BotInfo& botInfo);
    bool lineSegmentHasCollisionWithLineSegments(CollisionDetector& collisionDetector, LineSegment& a, QVector<LineSegment> lineSegments);

private:
    State m_state = State::Stopped;
    int m_tick = 0;
    Maze m_maze;
    QMap<int, BotInfo> m_bots;
    QVector<ActionItem> m_actionItems;
    QVector<QPoint> m_availableActionItemLocations;
};

#endif // GAME_H
