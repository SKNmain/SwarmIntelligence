#pragma once
#include "RandGen.h"
#include <ctime>

class Maze;
class MazeGenerator_I
{
   
public:
   ~MazeGenerator_I() = default;

   virtual Maze* generateMaze(uint32_t width, uint32_t height) = 0;
   virtual Maze* generateMazeStepByStep(uint32_t width, uint32_t height) = 0;
   virtual void nextMazeGenerationStep(Maze* maze) = 0;


protected:

   RandGen<std::knuth_b> rand{ time(NULL) };
};
