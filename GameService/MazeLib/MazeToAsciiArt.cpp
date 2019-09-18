// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "MazeToAsciiArt.h"

#include <QDebug>

static QSize cellArtSize = QSize(9, 4);

void drawCell(const Maze& maze, const QPoint& position, QVector<QVector<QChar>>& chars)
{
    auto cell = maze.getCell(position);
    int charactersHorizontallyPerCell = cellArtSize.width();
    int linesPerCell = cellArtSize.height();
    auto top = (maze.getLayout().height() - position.y() - 1)*linesPerCell;
    auto bottom = top + cellArtSize.height() - 1;
    auto left = position.x()*charactersHorizontallyPerCell;
    auto right = left + cellArtSize.width() - 1;

    // Vertical walls
    for (int y = 0; y < cellArtSize.height(); y++)
    {
        auto hasLeftWall = cell.walls.contains(MazeCell::Side::Left);
        if (hasLeftWall)
        {
            chars[top + y][left] = '|';
        }
        auto hasRightWall = cell.walls.contains(MazeCell::Side::Right);
        if (hasRightWall)
        {
            chars[top + y][right - 1] = '|';
        }
    }
    // Horizontal walls
    for (int x = 0; x < cellArtSize.width() - 1; x++)
    {
        auto hasTopWall = cell.walls.contains(MazeCell::Side::Top);
        if (hasTopWall)
        {
            chars[top][left + x] = '-';
        }
        auto hasBottomWall = cell.walls.contains(MazeCell::Side::Bottom);
        if (hasBottomWall)
        {
            chars[bottom][left + x] = '-';
        }
    }
    chars[top][left] = '+';
    chars[top][right - 1] = '+';
    chars[bottom][left] = '+';
    chars[bottom][right - 1] = '+';
}

QVector<QVector<QChar>> MazeToAsciiArt::mazeToAsciiArt(const Maze& maze)
{
    QVector<QVector<QChar>> chars(cellArtSize.height()*maze.getLayout().height(), QVector<QChar>(cellArtSize.width()*maze.getLayout().width(), ' '));
    for (int row = 0; row < maze.getLayout().height(); row++)
    {
        for (int column = 0; column < maze.getLayout().width(); column++)
        {
            drawCell(maze, QPoint(column, row), chars);
        }
    }
    return chars;
}

void MazeToAsciiArt::drawToConsole(const Maze& maze)
{
    auto chars = MazeToAsciiArt::mazeToAsciiArt(maze);
    for (auto line : chars)
    {
        QDebug deb = qDebug().noquote().nospace();
        for (auto character : line)
        {
            deb  << character;
        }
    }
}
