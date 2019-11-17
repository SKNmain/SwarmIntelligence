#pragma once
#include <QColor>
#include <vector>
#include <stack>
#include <Marker.h>

class Maze
{
public:
   Maze(uint32_t width, uint32_t height, uint32_t tileSize = 10, uint32_t pathWidth = 3, uint32_t markerSize = 20);
   void generateStep();

   enum CellIdentifier
   {
      CELL_PATH_N = 0X01,
      CELL_PATH_E = 0X02,
      CELL_PATH_S = 0X04,
      CELL_PATH_W = 0X08,
      CELL_VISITED = 0X10,
      CELL_SHORTEST = 0X12,

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
   std::pair<int, int> getStartingPoint() const;
   std::pair<int, int> getEndPoint() const;
   bool isMazeDone = false;

private:
   uint32_t width;
   uint32_t height;
   uint32_t tileSize;
   uint32_t pathWidth;

   uint32_t markerSize;

   std::pair<int, int> startingPoint;
   std::pair<int, int> endPoint;
   uint32_t numberOfVisitedCells = 0;
   std::stack<std::pair<int, int>> stack;
   std::vector<std::vector<int>> mazeArray;

   std::vector<Marker> markers;
};

