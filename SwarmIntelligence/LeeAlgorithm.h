#pragma once
#include <vector>
#include "Maze.h"
#include "MazeSolver_I.h"

class LeeAlgorithm : public MazeSolver_I
{
public:
   LeeAlgorithm();
   ~LeeAlgorithm();

   void solveMaze(Maze* maze) override;
  
   std::vector<std::vector<int>> getLeesArray() const;

private:
   bool isValid(int curX, int curY, int nextX, int nextY);
   void setVector(std::vector<std::vector<int>>& vector);

   const Maze* maze = nullptr;
   std::vector<std::vector<int>> shortestWay;
   std::vector<std::vector<int>> leeArray;
};

