// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include <random>

#include "Maze.h"

#ifndef MAZEPOSTPROCESSING_H
#define MAZEPOSTPROCESSING_H

class MazePostProcessing
{
public:
    MazePostProcessing();
    void removeRandomWallInInnerRegionOfMaze(Maze& maze, int numberOfCells);
    void removeDeadEndsInInnerRegionOfMaze(Maze& maze);

private:
    void removeRandomWall(Maze& maze, const QPoint& position);
    void breakOpen(Maze& maze, const QPoint& from, const QPoint& to);
    void breakOpen(Maze& maze, const QPoint& from, MazeCell::Side side);

private:
    std::random_device m_device;
    std::mt19937 m_gen;
};

#endif // MAZEPOSTPROCESSING_H
