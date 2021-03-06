#pragma once
#include "Maze.h"
#include "MazeGenerator_I.h"

class StackMazeGenerator : public MazeGenerator_I
{
public:

   Maze* generateMaze(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth) override;
   Maze* generateMazeStepByStep(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth) override;
   void nextMazeGenerationStep(Maze* maze) override;

private:
   void generateStep(Maze* maze);
   uint32_t numberOfVisitedCells = 0;

   std::stack<std::pair<int, int>> stack;
};

