// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Maze.h"

#ifndef MAZEGENERATORBASE_H
#define MAZEGENERATORBASE_H

class MazeGenerator
{
public:
    virtual ~MazeGenerator() = default;
    virtual Maze generateMaze(const QSize& layout) = 0;
};

#endif // MAZEGENERATORBASE_H
