// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Game.h"

#include <BacktrackMazeGenerator.h>
#include <MazePostProcessing.h>
#include <MazeToAsciiArt.h>
#include <QJsonArray>
#include <QDebug>
#include <QRandomGenerator>

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
    for (auto x = 0; x < m_maze.getLayout().width(); x++)
    {
        for (auto y = 0; y < m_maze.getLayout().height(); y++)
        {
            m_availableActionItemLocations << QPoint(x, y);
        }
    }
    for (auto type : options.numberOfActionItems.keys())
    {
        for (auto number = 0; number < options.numberOfActionItems[type]; number++)
        {
            placeActionItem(type);
        }
    }
}

void Game::placeActionItem(GameOptions::ActionItemType type)
{
    if (m_availableActionItemLocations.length() >= 1)
    {
        int randomLocation = QRandomGenerator::global()->bounded(m_availableActionItemLocations.length());
        auto location = m_availableActionItemLocations.takeAt(randomLocation);
        m_actionItems << ActionItem {type, QPointF((location.x()+0.5)/m_maze.getLayout().width(), (location.y()+0.5)/m_maze.getLayout().height()), location, ++ActionItem::nextId};
    }
}

void Game::handleTick()
{
    if (m_state == State::Running)
    {
        m_tick++;
        handleCollisions();
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
                                        {"y", actionItem.location.y()},
                                        {"id", actionItem.id}};
    }
//    if (m_tick % 30 == 0)
//    {
        auto last = m_actionItems.takeLast();
        m_availableActionItemLocations << last.cellIndex;
        placeActionItem(last.type);
//    }

    jsonState["actionItems"] = actionItemsArray;
    return jsonState;
}

void Game::handleCollisions()
{
    // Per bot: If timer not running => subtract points and set reset timer (QElapsedTimer?, maybe sharable with a ghost potion?)
    CollisionDetector collisionDetector;

    double widthPerCell = 1.0/m_maze.getLayout().width();
    double heightPerCell = 1.0/m_maze.getLayout().height();

    for (auto botInfo : m_bots)
    {
        handleCollisionPerBotInfo(collisionDetector, widthPerCell, heightPerCell, botInfo);


    }
}

void Game::handleCollisionPerBotInfo(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, BotInfo& botInfo)
{
    QVector<LineSegment> botLineSegments = getBotLineSegments(botInfo);

    if (hasWallCollision(collisionDetector, widthPerCell, heightPerCell, botLineSegments))
    {
        //substract points or??
    }

}

QVector<LineSegment> Game::getBotLineSegments(BotInfo& botInfo)
{
    QPointF rightForward =  botInfo.location + botInfo.forward.toPointF() + botInfo.right.toPointF();
    QPointF leftForward =  botInfo.location + botInfo.forward.toPointF() - botInfo.right.toPointF();
    QPointF rightBackward =  botInfo.location - botInfo.forward.toPointF() + botInfo.right.toPointF();
    QPointF leftBackward =  botInfo.location - botInfo.forward.toPointF() - botInfo.right.toPointF();

    LineSegment frontBot(leftForward, rightForward);
    LineSegment backBot(leftBackward, rightBackward);
    LineSegment rightBot(rightBackward, rightForward);
    LineSegment leftBot(leftForward, leftBackward);

    return {frontBot, backBot, rightBot, leftBot};
}

void Game::handleActionItems()
{
    // Per bot: If collision with actionItem => Handle effects & remove item & add position to available position list & place new item (in different spot?)

    // Slack conversation:
    //Spiketrap: penalty of 1 point (-1), has iframes after getting hit (10 seconds ?)
    //Bottle: can pass through a wall once
    //Testtube: can pass through a spiketrap once
    //Coin: 1 point
    //TreasureChest: 10 points
    //EmptyChest: 0 points
    //MimicChest: penalty of 5 points (-5)
}

bool Game::hasWallCollision(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments)
{
    for (auto y = 0; y < m_maze.getLayout().height(); y++)
    {
        for (auto x = 0; x < m_maze.getLayout().width(); x++)
        {
            MazeCell mazeCell = m_maze.getCell(QPoint(x, y));
            for (auto wall : mazeCell.walls)
            {
                switch (wall)
                {
                    case MazeCell::Side::Top:
                    {
                        LineSegment topWall(QPointF(x, y + heightPerCell), QPointF(x + widthPerCell, y + heightPerCell));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, topWall, botLineSegments))
                        {
                            return true;
                        }
                        break;
                    }
                    case MazeCell::Side::Left:
                    {
                        LineSegment leftWall(QPointF(x, y), QPointF(x, y+heightPerCell));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, leftWall, botLineSegments))
                        {
                            return true;
                        }
                    }
                    case MazeCell::Side::Bottom:
                    {
                        LineSegment bottomWall(QPointF(x, y), QPointF(x +widthPerCell, y));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, bottomWall, botLineSegments))
                        {
                            return true;
                        }
                    }
                    case MazeCell::Side::Right:
                    {
                        LineSegment rightWall(QPointF(x +widthPerCell, y), QPointF(x +widthPerCell, y+heightPerCell));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, rightWall, botLineSegments))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Game::lineSegmentHasCollisionWithLineSegments(CollisionDetector& collisionDetector, LineSegment& a, QVector<LineSegment> lineSegments)
{
    for (auto lineSegment : lineSegments)
    {
           if (collisionDetector.collide(lineSegment, a) )
           {
                 return true;
           }
    }
    return false;
}


