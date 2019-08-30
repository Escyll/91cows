// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "MazePostProcessing.h"

MazePostProcessing::MazePostProcessing()
    : m_gen(m_device())
{
}

void MazePostProcessing::removeRandomWall(Maze& maze, const QPoint& position)
{
    auto& cell = maze[position];
    QVector<MazeCell::Side> possibleWallsToRemove;
    if (position.x() > 0 && cell.walls.contains(MazeCell::Side::Left))
    {
        possibleWallsToRemove << MazeCell::Side::Left;
    }
    if (position.x() < maze.getLayout().width() - 1 && cell.walls.contains(MazeCell::Side::Right))
    {
        possibleWallsToRemove << MazeCell::Side::Right;
    }
    if (position.y() > 0 && cell.walls.contains(MazeCell::Side::Top))
    {
        possibleWallsToRemove << MazeCell::Side::Top;
    }
    if (position.y() < maze.getLayout().height() - 1 && cell.walls.contains(MazeCell::Side::Bottom))
    {
        possibleWallsToRemove << MazeCell::Side::Bottom;
    }
    if (possibleWallsToRemove.count() <= 0)
    {
        return;
    }
    std::uniform_int_distribution<> destructWallDist(0, possibleWallsToRemove.count() - 1);
    breakOpen(maze, position, possibleWallsToRemove[destructWallDist(m_gen)]);
}

void MazePostProcessing::removeRandomWallInInnerRegionOfMaze(Maze& maze, int numberOfCells)
{
    std::uniform_int_distribution<> xPostionDist(1, maze.getLayout().width() - 2);
    std::uniform_int_distribution<> yPostionDist(1, maze.getLayout().height() - 2);
    for (int i = 0; i < numberOfCells; i++)
    {
        removeRandomWall(maze, QPoint(xPostionDist(m_gen), yPostionDist(m_gen)));

    }
}

void MazePostProcessing::removeDeadEndsInInnerRegionOfMaze(Maze& maze)
{
    for (int x = 1; x < maze.getLayout().width() - 1; x++)
    {
        for (int y = 1; y < maze.getLayout().height() - 1; y++)
        {
            QPoint position(x, y);
            auto& cell = maze[position];
            if (cell.walls.size() == 3)
            {
                removeRandomWall(maze, position);
            }
        }
    }
}

void MazePostProcessing::breakOpen(Maze& maze, const QPoint& from, const QPoint& to)
{
    // Neighbour to the right
    if (to.x() == from.x() + 1)
    {
        maze[from].walls.removeAll(MazeCell::Side::Right);
        maze[to].walls.removeAll(MazeCell::Side::Left);
        return;
    }
    // Neighbour to the left
    if (to.x() == from.x() - 1)
    {
        maze[from].walls.removeAll(MazeCell::Side::Left);
        maze[to].walls.removeAll(MazeCell::Side::Right);
        return;
    }
    // Neighbour to the top
    if (to.y() == from.y() - 1)
    {
        maze[from].walls.removeAll(MazeCell::Side::Top);
        maze[to].walls.removeAll(MazeCell::Side::Bottom);
        return;
    }
    // Neighbour to the bottom
    if (to.y() == from.y() + 1)
    {
        maze[from].walls.removeAll(MazeCell::Side::Bottom);
        maze[to].walls.removeAll(MazeCell::Side::Top);
        return;
    }
    throw std::logic_error("These are no neightbours");
}

void MazePostProcessing::breakOpen(Maze& maze, const QPoint& from, MazeCell::Side side)
{
    switch (side)
    {
    case MazeCell::Side::Top:
        breakOpen(maze, from, QPoint(from.x(), from.y() - 1));
        break;
    case MazeCell::Side::Left:
        breakOpen(maze, from, QPoint(from.x() - 1, from.y()));
        break;
    case MazeCell::Side::Bottom:
        breakOpen(maze, from, QPoint(from.x(), from.y() + 1));
        break;
    case MazeCell::Side::Right:
        breakOpen(maze, from, QPoint(from.x() + 1, from.y()));
        break;
    }
}
