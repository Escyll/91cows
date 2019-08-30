// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef MAZETOASCIIART_H
#define MAZETOASCIIART_H

#include "Maze.h"

#include <QChar>
#include <QVector>

class MazeToAsciiArt
{
public:
    static QVector<QVector<QChar>> mazeToAsciiArt(const Maze& maze);
    static void drawToConsole(const Maze& maze);
};

#endif // MAZETOASCIIART_H
