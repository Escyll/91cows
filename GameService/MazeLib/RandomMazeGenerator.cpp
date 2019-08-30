// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include <random>

#include "RandomMazeGenerator.h"

static QVector<MazeCell::Side> walls = { MazeCell::Side::Top, MazeCell::Side::Left, MazeCell::Side::Bottom, MazeCell::Side::Right};
Maze RandomMazeGenerator::generateMaze(const QSize& layout)
{
    QVector<MazeCell> cells;
    for (auto row = 0; row < layout.height(); row++)
    {
        for (auto column = 0; column < layout.width(); column++)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> wallDist(0, 3);
            std::uniform_int_distribution<> nWallDist(0, 3);

            MazeCell cell;
            auto nWalls = nWallDist(gen);
            for (auto i = 0; i < nWalls; i++)
            {
                cell.walls << walls[wallDist(gen)];
            }

            cells << cell;
        }
    }
    return Maze(layout, cells);
}
