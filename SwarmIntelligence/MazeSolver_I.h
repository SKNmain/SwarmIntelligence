#pragma once
#include <vector>
class Maze;

class MazeSolver_I
{
public:
   virtual ~MazeSolver_I() = default;

   virtual void solveMaze(Maze* maze) = 0;

protected:
};
