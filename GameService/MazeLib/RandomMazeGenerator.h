// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "Maze.h"
#include "MazeGenerator.h"

class RandomMazeGenerator : public MazeGenerator
{
public:
    Maze generateMaze(const QSize& layout) override;
};

#endif // MAZEGENERATOR_H
