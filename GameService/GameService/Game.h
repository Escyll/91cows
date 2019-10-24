// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef GAME_H
#define GAME_H

#include "ActionItem.h"
#include "BotInfo.h"
#include "GameOptions.h"
#include "TeamSettings.h"
#include <CollisionDetector.h>

#include <Maze.h>
#include <QJsonObject>
#include <QMap>
#include <QSharedPointer>
#include <QTimer>
#include <QVector2D>

class Game
{
public:
    Game();
    Game(GameOptions options);
    enum class State {
        Waiting = 0,
        Running = 1,
        Stopped = 2
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
    bool hasActionItemCollition(const CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, const QVector<LineSegment>& botLineSegments, ActionItem& collideActionItem);
    void handleActionItemCollision(BotInfo& botInfo, const ActionItem& actionItem);
    QVector<LineSegment> getBotLineSegments(BotInfo& botInfo);
    bool lineSegmentHasCollisionWithLineSegments(const CollisionDetector& collisionDetector, const LineSegment& a, const QVector<LineSegment>& lineSegments);

private:
    State m_state = State::Stopped;
    int m_tick = 0;
    static int m_gameId;
    Maze m_maze;
    QMap<int, BotInfo> m_bots;
    QVector<ActionItem> m_actionItems;
    QVector<QPoint> m_availableActionItemLocations;
    QSharedPointer<QTimer> m_gameTimer;
    int m_remainingTime;

    QMap<int, bool> m_BottleBotMap;
    QMap<int, bool> m_TesttubeBotMap;
    QMap<int, bool> m_WallCollisionBotMap;
    QMap<int, bool> m_ActionItemsCollisionBotMap;
};

#endif // GAME_H
