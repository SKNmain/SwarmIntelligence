#pragma once
#include <QColor>
#include <vector>
#include <stack>
#include <Marker.h>
#include "RandGen.h"
#include <ctime>

class Maze
{
public:
   Maze(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth, uint32_t markerSize);
   void generateStep();

   enum CellIdentifier
   {
      CELL_PATH_N = 1 << 0,
      CELL_PATH_E = 1 << 1,
      CELL_PATH_S = 1 << 2,
      CELL_PATH_W = 1 << 3,
      CELL_VISITED = 1 << 4,
   };

   void generateMaze();
   void removeMarkers();

   std::vector<std::vector<int>> getMazeArray() const;
   std::vector<Marker> getMarkers() const;
   uint32_t getHeight() const;
   uint32_t getWidth() const;
   uint32_t getMarkerSize() const;
   uint32_t getTileSize() const;
   uint32_t getPathWidth() const;


   bool isMazeGenerationFinished() const;

private:
   bool isMazeDone = false;
   uint32_t width;
   uint32_t height;
   uint32_t tileSize;
   uint32_t pathWidth;

   uint32_t markerSize;


   uint32_t numberOfVisitedCells = 0;
   std::stack<std::pair<int, int>> stack;
   std::vector<std::vector<int>> mazeArray;

   std::vector<Marker> markers;
   RandGen<std::knuth_b> rand{ time(NULL) };
};

