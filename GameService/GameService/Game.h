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
    QJsonArray getBotJsonArray();
    void insertSharedGameState(QJsonObject& jsonState);
    void placeActionItem(GameOptions::ActionItemType type);

private:
    void handleGameLoop();
    void handleGameLoopPerBotInfo(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, BotInfo& botInfo);
    bool hasWallCollision(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments);
    bool hasCornerWallCollision(CollisionDetector& collisionDetector, double xStart, double yStart, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments);
    void handleWallCollision(BotInfo& botInfo);
    bool hasActionItemCollition(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments, ActionItem& collideActionItem);
    void handleActionItemCollision(BotInfo& botInfo, ActionItem& actionItem);
    QVector<LineSegment> getBotLineSegments(BotInfo& botInfo);
    bool lineSegmentHasCollisionWithLineSegments(CollisionDetector& collisionDetector, LineSegment& a, QVector<LineSegment> lineSegments);

private:
    State m_state = State::Stopped;
    int m_tick = 0;
    Maze m_maze;
    QMap<int, BotInfo> m_bots;
    QVector<ActionItem> m_actionItems;
    QVector<QPoint> m_availableActionItemLocations;

    QMap<int, bool> m_BottleBotMap;
    QMap<int, bool> m_TesttubeBotMap;
    QMap<int, bool> m_WallCollisionBotMap;
    QMap<int, int> m_PointsBotMap;
};

#endif // GAME_H
