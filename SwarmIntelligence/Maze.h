#pragma once
#include <QColor>
#include <vector>
#include <stack>

class Maze
{
public:
   Maze(uint32_t width, uint32_t height, uint32_t tileWidth = 20, uint32_t tileHeight = 20, uint32_t pathWidth = 3);

   enum CellIdentifier { CELL_PATH_N = 0X01, CELL_PATH_E = 0X02, CELL_PATH_S = 0X04, CELL_PATH_W = 0X08, CELL_VISITED = 0X10, };

   void generateMaze();
   int numberOfVisitedCells = 0;
   std::stack<std::pair<int, int>> stack;
   std::vector<std::vector<int>> getMazeArray() const;
   uint32_t getHeight() const;
   uint32_t getWidth() const;
   uint32_t getTileHeight() const;
   uint32_t getTileWidth() const;
   uint32_t getPathWidth() const;
   bool isMazeDone = false;
private:
   uint32_t width;
   uint32_t height;
   uint32_t tileWidth;
   uint32_t tileHeight;
   uint32_t pathWidth;

   
   std::vector<std::vector<int>> mazeArray;
};

