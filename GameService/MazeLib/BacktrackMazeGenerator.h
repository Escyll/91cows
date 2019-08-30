// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BACKTRACKMAZEGENERATOR_H
#define BACKTRACKMAZEGENERATOR_H

#include <random>

#include "MazeGenerator.h"

struct VisitableMazeCell
{
    VisitableMazeCell() = default;
    VisitableMazeCell(const QVector<MazeCell::Side>& cellWalls, bool visited);
    MazeCell mazeCell;
    bool visited = false;
};

struct VisitableMaze
{
public:
    VisitableMaze(const QSize& layout, const QVector<VisitableMazeCell>& cells);
    bool hasUnvisitedNeighbours(const QPoint& position) const;
    bool isWithinBounds(const QPoint& position) const;
    const VisitableMazeCell& operator [](const QPoint& position) const;
    VisitableMazeCell& operator[](const QPoint& position);
    const VisitableMazeCell& getCell(const QPoint& position) const;
    VisitableMazeCell& getCell(const QPoint& position);
    QVector<QPoint> getUnvisitedNeightbours(const QPoint& position) const;
    void breakOpen(const QPoint& from, const QPoint& to);

    QSize layout;
    QVector<VisitableMazeCell> cells;
private:
};

class BacktrackMazeGenerator : public MazeGenerator
{
public:
    BacktrackMazeGenerator();
    Maze generateMaze(const QSize& layout) override;
private:
    void backTrack(VisitableMaze& maze, const QPoint& position);

    std::random_device m_device;
    std::mt19937 m_gen;
};


#endif // BACKTRACKMAZEGENERATOR_H
