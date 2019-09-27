// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "Maze.h"

#include <QJsonArray>

Maze::Maze()
{
}

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

QJsonArray Maze::toJson()
{
    QJsonArray rows;
    for (auto y = 0; y < getLayout().height(); y++)
    {
        QJsonArray column;
        for (auto x = 0; x < getLayout().width(); x++)
        {
            column << getCell(QPoint(x, y)).toJson();
        }
        rows << column;
    }
    return rows;
}

MazeCell::MazeCell(const QVector<Side>& walls)
    : walls(walls)
{
}

QJsonObject MazeCell::toJson()
{
    switch (walls.size())
    {
    case 1:
        switch (walls[0])
        {
        case Side::Top:
            return QJsonObject({{"type", 1}, {"orientation", 270}});
        case Side::Left:
            return QJsonObject({{"type", 1}, {"orientation", 0}});
        case Side::Bottom:
            return QJsonObject({{"type", 1}, {"orientation", 90}});
        case Side::Right:
            return QJsonObject({{"type", 1}, {"orientation", 180}});
        }
    case 2:
        if (walls.contains(Side::Left) && walls.contains(Side::Right))
        {
            return QJsonObject({{"type", 2}, {"orientation", 0}});
        }
        else if (walls.contains(Side::Top) && walls.contains(Side::Bottom))
        {
            return QJsonObject({{"type", 2}, {"orientation", 90}});
        }
        else if (walls.contains(Side::Left))
        {
            if (walls.contains(Side::Top))
            {
                return QJsonObject({{"type", 3}, {"orientation", 0}});
            }
            else
            {
                return QJsonObject({{"type", 3}, {"orientation", 90}});
            }
        }
        else
        {
            if (walls.contains(Side::Top))
            {
                return QJsonObject({{"type", 3}, {"orientation", 270}});
            }
            else
            {
                return QJsonObject({{"type", 3}, {"orientation", 180}});
            }
        }
    case 3:
        if (!walls.contains(Side::Bottom))
        {
            return QJsonObject({{"type", 4}, {"orientation", 0}});
        }
        else if (!walls.contains(Side::Right))
        {
            return QJsonObject({{"type", 4}, {"orientation", 90}});
        }
        else if (!walls.contains(Side::Top))
        {
            return QJsonObject({{"type", 4}, {"orientation", 180}});
        }
        else
        {
            return QJsonObject({{"type", 4}, {"orientation", 270}});
        }
    default:
        return QJsonObject({{"type", 0}, {"orientation", 0}});
    }
}
