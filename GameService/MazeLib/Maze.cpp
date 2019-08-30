// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Maze.h"

Maze::Maze(const QSize& layout, const QVector<MazeCell>& cells)
    : m_layout(layout)
    , m_cells(cells)
{
    if (m_layout.width()*m_layout.height() != m_cells.count())
    {
        throw std::logic_error("Size of layout and size of vector of cells are different");
    }
}

QVector<MazeCell> Maze::getCells() const
{
    return m_cells;
}

MazeCell& Maze::operator[](const QPoint& position)
{
    return getCell(position);
}

const MazeCell& Maze::getCell(const QPoint& position) const
{
    return m_cells[position.y()*m_layout.width() + position.x()];
}

const MazeCell& Maze::operator[](const QPoint& position) const
{
    return getCell(position);
}

MazeCell& Maze::getCell(const QPoint& position)
{
    return m_cells[position.y()*m_layout.width() + position.x()];
}

QSize Maze::getLayout() const
{
    return m_layout;
}

MazeCell::MazeCell(const QVector<MazeCell::Side>& walls)
    : walls(walls)
{
}
