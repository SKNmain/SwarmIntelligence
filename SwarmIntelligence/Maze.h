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
   

   enum CellIdentifier
   {
      CELL_PATH_N = 1 << 0,
      CELL_PATH_E = 1 << 1,
      CELL_PATH_S = 1 << 2,
      CELL_PATH_W = 1 << 3,
      CELL_VISITED = 1 << 4,
      CELL_SHORTEST = 1 << 5,
      CELL_START = 1 << 6,
      CELL_END = 1 << 7,
   };

   void removeMarkers();
   std::vector<Marker> getMarkers() const;

   uint32_t getHeight() const;
   uint32_t getWidth() const;
   uint32_t getMarkerSize() const;
   uint32_t getTileSize() const;
   uint32_t getPathWidth() const;
   std::pair<int, int> getStartingPoint() const;
   std::pair<int, int> getEndPoint() const;

   void finishedGenerating();
   bool isGeneratingFinished() const;

   std::vector<std::vector<int>> mazeArray;
   std::vector<std::vector<int>> shortestWayArray;
   std::vector<Marker> markers;

private:
   uint32_t width;
   uint32_t height;
   uint32_t tileSize;
   uint32_t pathWidth;

   uint32_t markerSize;

   std::pair<int, int> startingPoint;
   std::pair<int, int> endPoint;
   bool generatingFinished = false;
};

