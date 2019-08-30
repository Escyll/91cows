// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include <QPoint>
#include <QSize>
#include <QVector>

#ifndef MAZE_H
#define MAZE_H

class MazeCell
{
public:
    enum class Side {
        Top,
        Left,
        Bottom,
        Right
    };
    MazeCell() = default;
    MazeCell(const QVector<Side>& walls);
    virtual ~MazeCell() = default;
    QVector<Side> walls;
};

class Maze
{
public:
    Maze(const QSize& layout, const QVector<MazeCell>& cells);

    QVector<MazeCell> getCells() const;
    MazeCell& getCell(const QPoint& position);
    MazeCell& operator[](const QPoint& position);
    const MazeCell& getCell(const QPoint& position) const;
    const MazeCell& operator[](const QPoint& position) const;
    QSize getLayout() const;

private:
    QSize m_layout;
    QVector<MazeCell> m_cells;
};

#endif // MAZE_H
