#pragma once
#include <vector>
#include "Maze.h"

class LeeAlgorithm
{
public:
   LeeAlgorithm(const Maze &givenMaze);
   ~LeeAlgorithm();

   void solveMaze();
   std::vector<std::vector<int>> getLeesArray() const;
   std::vector<std::vector<int>> getArrayToDraw() const;
  

private:
   int width;
   int height;
   bool LeeAlgorithm::isValid(int curX, int curY, int nextX, int nextY);
   std::vector<std::vector<int>> givenArray;
   std::vector<std::vector<int>> leeArray;
   std::vector<std::vector<int>> showArray;
   void LeeAlgorithm::setVector(std::vector<std::vector<int>> vector);


   std::pair<int, int> startingPoint;
   std::pair<int, int> endPoint;

};

