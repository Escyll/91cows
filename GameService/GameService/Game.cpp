// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Game.h"

#include <BacktrackMazeGenerator.h>
#include <MazePostProcessing.h>
#include <MazeToAsciiArt.h>
#include <QJsonArray>
#include <QDebug>
#include <QRandomGenerator>

int Game::m_gameId = 0;

Game::Game()
{
    m_tick = 0;
    m_bots.clear();
}

Game::Game(GameOptions options)
{
    m_bots.clear();
    m_tick = 0;
    ++m_gameId;
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
        handleGameLoop();
    }
}

void Game::start()
{ 
    m_state = State::Running;
}

void Game::stop()
{
    m_state = State::Stopped;
}

void Game::setBotLocations(const QVector<BotInfo>& bots)
{
    for (auto bot : bots)
    {
        if (!m_bots.contains(bot.arucoId))
        {
            if (m_state == State::Waiting)
            {
                m_bots[bot.arucoId] = bot;
            }
        }
        else
        {
            m_bots[bot.arucoId].color = bot.color;
            m_bots[bot.arucoId].name = bot.name;
            m_bots[bot.arucoId].location = bot.location;
            m_bots[bot.arucoId].forward = bot.forward;
            m_bots[bot.arucoId].right = bot.right;
        }
    }
}

QJsonArray Game::getBotJsonArray()
{
    QJsonArray botArray;
    for (auto botInfo : m_bots)
    {
        QJsonObject bot;
        bot["arucoId"] = botInfo.arucoId;
        bot["name"] = botInfo.name;
        bot["position"] = QJsonArray() << botInfo.location.x() << botInfo.location.y();
        bot["score"] =  botInfo.score;
        bot["color"] = static_cast<int>(botInfo.color.rgba());
        bot["forward"] = QJsonArray() << static_cast<double>(botInfo.forward.x()) << static_cast<double>(botInfo.forward.y());
        bot["right"] = QJsonArray() << static_cast<double>(botInfo.right.x()) << static_cast<double>(botInfo.right.y());
        botArray << bot;
    }
    return botArray;
}

void Game::insertSharedGameState(QJsonObject& jsonState)
{
    jsonState["gameId"] =  m_gameId;
    jsonState["gameState"] = static_cast<int>(m_state);
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
    QJsonArray actionItemsArray;
    for (auto actionItem : m_actionItems)
    {
        if (actionItem.type == GameOptions::ActionItemType::EmptyChest || actionItem.type == GameOptions::ActionItemType::MimicChest)
        {
            actionItem.type = GameOptions::ActionItemType::TreasureChest;
        }
        actionItemsArray << QJsonObject{{"type", GameOptions::actionItemTypeToString(actionItem.type)},
                                        {"x", actionItem.location.x()},
                                        {"y", actionItem.location.y()},
                                        {"id", actionItem.id}};
    }
    jsonState["actionItems"] = actionItemsArray;
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
    jsonState["actionItems"] = actionItemsArray;
    return jsonState;
}

void Game::handleGameLoop()
{
    // Per bot: If timer not running => subtract points and set reset timer (QElapsedTimer?, maybe sharable with a ghost potion?)
    CollisionDetector collisionDetector;

    double widthPerCell = 1.0/m_maze.getLayout().width();
    double heightPerCell = 1.0/m_maze.getLayout().height();

    for (auto& botInfo : m_bots)
    {
        handleGameLoopPerBotInfo(collisionDetector, widthPerCell, heightPerCell, botInfo);
    }
}

void Game::handleGameLoopPerBotInfo(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, BotInfo& botInfo)
{
    QVector<LineSegment> botLineSegments = getBotLineSegments(botInfo);

    if (hasWallCollision(collisionDetector, widthPerCell, heightPerCell, botLineSegments))
    {
        if (!m_BottleBotMap.contains(botInfo.arucoId))
        {
            handleWallCollision(botInfo);
        }
        m_WallCollisionBotMap[botInfo.arucoId] = true;
    }
    else if (m_WallCollisionBotMap.contains(botInfo.arucoId))
    {
        m_WallCollisionBotMap.remove(botInfo.arucoId);
        m_BottleBotMap.remove(botInfo.arucoId);
    }

    ActionItem collideActionItem;
    if (hasActionItemCollition(collisionDetector, widthPerCell, heightPerCell, botLineSegments, collideActionItem))
    {
        if (!m_TesttubeBotMap.contains(botInfo.arucoId) || collideActionItem.type != GameOptions::ActionItemType::SpikeTrap)
        {
            handleActionItemCollision(botInfo, collideActionItem);
        }
        m_ActionItemsCollisionBotMap[botInfo.arucoId] = true;
    }
    else if (m_ActionItemsCollisionBotMap.contains(botInfo.arucoId))
    {
        m_ActionItemsCollisionBotMap.remove(botInfo.arucoId);
        m_TesttubeBotMap.remove(botInfo.arucoId);
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

bool Game::hasWallCollision(CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments)
{
    for (auto y = 0; y < m_maze.getLayout().height(); y++)
    {
        for (auto x = 0; x < m_maze.getLayout().width(); x++)
        {
            MazeCell mazeCell = m_maze.getCell(QPoint(x, y));

            double xStart = x * widthPerCell;
            double yStart = y * heightPerCell;
            if (hasCornerWallCollision(collisionDetector, xStart, yStart, widthPerCell, heightPerCell, botLineSegments))
            {
                return true;
            }

            for (auto wall : mazeCell.walls)
            {
                switch (wall)
                {
                    case MazeCell::Side::Top:
                    {
                        LineSegment topWall(QPointF(xStart, yStart  + heightPerCell), QPointF(xStart + widthPerCell, yStart + heightPerCell));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, topWall, botLineSegments))
                        {
                            return true;
                        }
                        break;
                    }
                    case MazeCell::Side::Left:
                    {
                        LineSegment leftWall(QPointF(xStart, yStart), QPointF(xStart, yStart+heightPerCell));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, leftWall, botLineSegments))
                        {
                            return true;
                        }
                        break;
                    }
                    case MazeCell::Side::Bottom:
                    {
                        LineSegment bottomWall(QPointF(xStart, yStart), QPointF(xStart +widthPerCell, yStart));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, bottomWall, botLineSegments))
                        {
                            return true;
                        }
                        break;
                    }
                    case MazeCell::Side::Right:
                    {
                        LineSegment rightWall(QPointF(xStart + widthPerCell, yStart), QPointF(xStart + widthPerCell, yStart+heightPerCell));
                        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, rightWall, botLineSegments))
                        {
                            return true;
                        }
                        break;
                    }
                }
            }

        }
    }
    return false;
}

bool Game::hasCornerWallCollision(CollisionDetector& collisionDetector, double xStart, double yStart, double widthPerCell, double heightPerCell, QVector<LineSegment> botLineSegments)
{
    double widthCornerWall = widthPerCell / 6.0;
    double heightCornerWall = heightPerCell / 6.0;

    LineSegment cornerBottomLeftToTopWall(QPointF(xStart, yStart), QPointF(xStart, yStart + heightCornerWall));
    LineSegment cornerBottomLeftToRightWall(QPointF(xStart, yStart), QPointF(xStart + widthCornerWall, yStart));

    LineSegment cornerTopLeftToBottomWall(QPointF(xStart, yStart + heightPerCell), QPointF(xStart, yStart + heightPerCell - heightCornerWall));
    LineSegment cornerTopLeftToRightWall(QPointF(xStart, yStart + heightPerCell), QPointF(xStart + widthCornerWall, yStart + heightPerCell));

    LineSegment cornerTopRightToBottomWall(QPointF(xStart + widthPerCell, yStart + heightPerCell), QPointF(xStart + widthPerCell, yStart + heightPerCell - heightCornerWall));
    LineSegment cornerTopRightToLeftWall(QPointF(xStart + widthPerCell, yStart + heightPerCell), QPointF(xStart + widthPerCell - widthCornerWall, yStart + heightPerCell));

    LineSegment cornerBottomRightToTopWall(QPointF(xStart + widthPerCell, yStart), QPointF(xStart + widthPerCell, yStart + heightCornerWall));
    LineSegment cornerBottomRightToLeftWall(QPointF(xStart + widthPerCell, yStart), QPointF(xStart + widthPerCell - widthCornerWall, yStart));

    QVector<LineSegment> cornerLineSegments =
    {
        cornerBottomLeftToTopWall, cornerBottomLeftToRightWall,
        cornerTopLeftToBottomWall, cornerTopLeftToRightWall,
        cornerTopRightToBottomWall, cornerTopRightToLeftWall,
        cornerBottomRightToTopWall, cornerBottomRightToLeftWall
    };

    for (auto cornerLineSegment : cornerLineSegments)
    {
        if (lineSegmentHasCollisionWithLineSegments(collisionDetector, cornerLineSegment, botLineSegments))
        {
            return true;
        }
    }
    return false;
}


void Game::handleWallCollision(BotInfo& botInfo)
{
    if (botInfo.wallColisionTimer.isValid() && botInfo.wallColisionTimer.elapsed() < 1000) return;
    botInfo.score--;
    botInfo.wallColisionTimer.start();
}


bool Game::hasActionItemCollition(const CollisionDetector& collisionDetector, double widthPerCell, double heightPerCell, const QVector<LineSegment>& botLineSegments, ActionItem& collideActionItem)
{
    double widthHalfActionItem = widthPerCell / 6.0;
    double heightHalfActionItem = heightPerCell / 6.0;

    for (auto actionItem : m_actionItems)
    {
        LineSegment topActionItem(QPointF(actionItem.location.x() - widthHalfActionItem, actionItem.location.y() + heightHalfActionItem),
                                  QPointF(actionItem.location.x() + widthHalfActionItem, actionItem.location.y() + heightHalfActionItem));
        LineSegment rightActionItem(QPointF(actionItem.location.x() + widthHalfActionItem, actionItem.location.y() + heightHalfActionItem),
                                  QPointF(actionItem.location.x() + widthHalfActionItem, actionItem.location.y() - heightHalfActionItem));
        LineSegment bottomActionItem(QPointF(actionItem.location.x() - widthHalfActionItem, actionItem.location.y() - heightHalfActionItem),
                                  QPointF(actionItem.location.x() - widthHalfActionItem, actionItem.location.y() - heightHalfActionItem));
        LineSegment leftActionItem(QPointF(actionItem.location.x() - widthHalfActionItem, actionItem.location.y() - heightHalfActionItem),
                                  QPointF(actionItem.location.x() - widthHalfActionItem, actionItem.location.y() + heightHalfActionItem));
        QVector<LineSegment> actionItemSegments = {topActionItem, rightActionItem, bottomActionItem, leftActionItem};
        for (auto actionItemSegment : actionItemSegments)
        {
            if (lineSegmentHasCollisionWithLineSegments(collisionDetector, actionItemSegment, botLineSegments))
            {
                collideActionItem = actionItem;
                return true;
            }
        }
    }

    return false;
}

void Game::handleActionItemCollision(BotInfo& botInfo, const ActionItem& actionItem)
{
    // Per bot: If collision with actionItem => Handle effects & remove item & add position to available position list & place new item (in different spot?)
    switch (actionItem.type)
    {
        case GameOptions::ActionItemType::Coin:
        {
            botInfo.score += 2;
            break;
        }
        case GameOptions::ActionItemType::Bottle:
        {
            m_BottleBotMap.insert(botInfo.arucoId, true);
            break;
        }
        case GameOptions::ActionItemType::TestTube:
        {
            m_TesttubeBotMap.insert(botInfo.arucoId, true);
            break;
        }
        case GameOptions::ActionItemType::SpikeTrap:
        {
            if (botInfo.spikeCollisionTimer.isValid() && botInfo.spikeCollisionTimer.elapsed() < 1000) return;
            botInfo.score--;
            botInfo.spikeCollisionTimer.start();
            break;
        }
        case GameOptions::ActionItemType::EmptyChest:
        {
            break;
        }
        case GameOptions::ActionItemType::MimicChest:
        {
            botInfo.score -= 5;
            break;
        }
        case GameOptions::ActionItemType::TreasureChest:
        {
            botInfo.score += 10;
            break;
        }
    }

    if (actionItem.type != GameOptions::ActionItemType::SpikeTrap)
    {
        auto cellIndex = actionItem.cellIndex;
        placeActionItem(actionItem.type);
        m_actionItems.removeAll(actionItem);
        m_availableActionItemLocations.append(cellIndex);
    }

    // Slack conversation:
    //Spiketrap: penalty of 1 point (-1), has iframes after getting hit (10 seconds ?)
    //Bottle: can pass through a wall once
    //Testtube: can pass through a spiketrap once
    //Coin: 1 point
    //TreasureChest: 10 points
    //EmptyChest: 0 points
    //MimicChest: penalty of 5 points (-5)

}

bool Game::lineSegmentHasCollisionWithLineSegments(const CollisionDetector& collisionDetector, const LineSegment& a, const QVector<LineSegment>& lineSegments)
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


