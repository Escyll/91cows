#include <QDebug>

#include "BacktrackMazeGenerator.h"
#include "RandomMazeGenerator.h"
#include "MazeToAsciiArt.h"
#include "MazePostProcessing.h"

int main(int /*argc*/, char */*argv*/[])
{
    BacktrackMazeGenerator generator;
    MazePostProcessing postProcessor;
    auto maze = generator.generateMaze(QSize(11, 11));
    postProcessor.removeDeadEndsInInnerRegionOfMaze(maze);
    postProcessor.removeRandomWallInInnerRegionOfMaze(maze, 20);
    MazeToAsciiArt::drawToConsole(maze);
}
