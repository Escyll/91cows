#include <QDebug>

#include "BacktrackMazeGenerator.h"
#include "RandomMazeGenerator.h"
#include "MazeToAsciiArt.h"
#include "MazePostProcessing.h"

int main(int /*argc*/, char */*argv*/[])
{
    BacktrackMazeGenerator generator;
    MazePostProcessing postProcessor;
    auto maze = generator.generateMaze(QSize(10, 10));
    postProcessor.removeDeadEndsInInnerRegionOfMaze(maze);
    postProcessor.removeRandomWallInInnerRegionOfMaze(maze, 15);
    MazeToAsciiArt::drawToConsole(maze);
}
