// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BacktrackMazeGenerator.h"

VisitableMaze generateFullyWalledMaze(const QSize& layout)
{
    QVector<VisitableMazeCell> visitableMazeCells(layout.width()*layout.height());
    for (int i = 0; i < layout.width()*layout.height(); i++)
    {
        visitableMazeCells[i] = VisitableMazeCell({MazeCell::Side::Top, MazeCell::Side::Left, MazeCell::Side::Bottom, MazeCell::Side::Right}, false);
    }
    return VisitableMaze(layout, visitableMazeCells);
}

Maze convertToMaze(const VisitableMaze& visitableMaze)
{
    QVector<MazeCell> cells;
    std::transform(visitableMaze.cells.begin(),
                   visitableMaze.cells.end(),
                   std::back_inserter(cells),
                   [](const VisitableMazeCell& visitableMazeCell) { return visitableMazeCell.mazeCell; });
    return Maze(visitableMaze.layout, cells);
}

BacktrackMazeGenerator::BacktrackMazeGenerator()
    : m_gen(m_device())
{
}

Maze BacktrackMazeGenerator::generateMaze(const QSize& layout)
{
    auto visitableMaze = generateFullyWalledMaze(layout);
    backTrack(visitableMaze, QPoint(0, 0));
    return convertToMaze(visitableMaze);
}

void BacktrackMazeGenerator::backTrack(VisitableMaze& maze, const QPoint& position)
{
    maze[position].visited = true;
    while(maze.hasUnvisitedNeighbours(position))
    {
        auto unvisitedNeighbours = maze.getUnvisitedNeightbours(position);
        std::uniform_int_distribution<> wallDist(0, unvisitedNeighbours.count() - 1);
        auto unvisitedNeighbour = unvisitedNeighbours[wallDist(m_gen)];
        maze.breakOpen(position, unvisitedNeighbour);
        backTrack(maze, unvisitedNeighbour);
    }
}

void VisitableMaze::breakOpen(const QPoint& from, const QPoint& to)
{
    // Neighbour to the right
    if (to.x() == from.x() + 1)
    {
        getCell(from).mazeCell.walls.removeAll(MazeCell::Side::Right);
        getCell(to).mazeCell.walls.removeAll(MazeCell::Side::Left);
        return;
    }
    // Neighbour to the left
    if (to.x() == from.x() - 1)
    {
        getCell(from).mazeCell.walls.removeAll(MazeCell::Side::Left);
        getCell(to).mazeCell.walls.removeAll(MazeCell::Side::Right);
        return;
    }
    // Neighbour to the top
    if (to.y() == from.y() - 1)
    {
        getCell(from).mazeCell.walls.removeAll(MazeCell::Side::Top);
        getCell(to).mazeCell.walls.removeAll(MazeCell::Side::Bottom);
        return;
    }
    // Neighbour to the bottom
    if (to.y() == from.y() + 1)
    {
        getCell(from).mazeCell.walls.removeAll(MazeCell::Side::Bottom);
        getCell(to).mazeCell.walls.removeAll(MazeCell::Side::Top);
        return;
    }
    throw std::logic_error("These are no neightbours");
}

VisitableMazeCell::VisitableMazeCell(const QVector<MazeCell::Side>& cellWalls, bool visited)
    : mazeCell(cellWalls)
    , visited(visited) {}

VisitableMaze::VisitableMaze(const QSize& layout, const QVector<VisitableMazeCell>& cells)
    : layout(layout)
    , cells(cells)
{
}

const VisitableMazeCell& VisitableMaze::getCell(const QPoint& position) const
{
    return cells[position.y()*layout.width() + position.x()];
}

VisitableMazeCell& VisitableMaze::getCell(const QPoint& position)
{
    return cells[position.y()*layout.width() + position.x()];
}

const VisitableMazeCell& VisitableMaze::operator[](const QPoint& position) const
{
    return getCell(position);
}

VisitableMazeCell& VisitableMaze::operator[](const QPoint& position)
{
    return getCell(position);
}

bool VisitableMaze::hasUnvisitedNeighbours(const QPoint& position) const
{
    auto topNeighbour = QPoint(position.x(), position.y() - 1);
    if (isWithinBounds(topNeighbour) && !getCell(topNeighbour).visited)
    {
        return true;
    }
    auto leftNeighbour = QPoint(position.x() - 1, position.y());
    if (isWithinBounds(leftNeighbour) && !getCell(leftNeighbour).visited)
    {
        return true;
    }
    auto bottomNeighbour = QPoint(position.x(), position.y() + 1);
    if (isWithinBounds(bottomNeighbour) && !getCell(bottomNeighbour).visited)
    {
        return true;
    }
    auto rightNeighbour = QPoint(position.x() + 1, position.y());
    if (isWithinBounds(rightNeighbour) && !getCell(rightNeighbour).visited)
    {
        return true;
    }
    return false;
}

QVector<QPoint> VisitableMaze::getUnvisitedNeightbours(const QPoint& position) const
{
    QVector<QPoint> possibleNeighbours;
    auto topNeighbour = QPoint(position.x(), position.y() - 1);
    if (isWithinBounds(topNeighbour) && !getCell(topNeighbour).visited)
    {
        possibleNeighbours << topNeighbour;
    }
    auto leftNeighbour = QPoint(position.x() - 1, position.y());
    if (isWithinBounds(leftNeighbour) && !getCell(leftNeighbour).visited)
    {
        possibleNeighbours << leftNeighbour;
    }
    auto bottomNeighbour = QPoint(position.x(), position.y() + 1);
    if (isWithinBounds(bottomNeighbour) && !getCell(bottomNeighbour).visited)
    {
        possibleNeighbours << bottomNeighbour;
    }
    auto rightNeighbour = QPoint(position.x() + 1, position.y());
    if (isWithinBounds(rightNeighbour) && !getCell(rightNeighbour).visited)
    {
        possibleNeighbours << rightNeighbour;
    }
    if (possibleNeighbours.isEmpty())
    {
        throw std::logic_error("No unvisited neighbour");
    }
    return possibleNeighbours;
}

bool VisitableMaze::isWithinBounds(const QPoint& position) const
{
    return 0 <= position.x() && position.x() < layout.width() &&
            0 <= position.y() && position.y() < layout.height();
}
