#include "Maze.h"
#include <ctime>
#include "RandGen.h"

Maze::Maze(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth, uint32_t markerSize)
{
   this->width = width;
   this->height = height;

   this->markerSize = markerSize;
   this->tileSize = tileSize;
   this->pathWidth = pathWidth;

   for (uint32_t y = 0; y < this->height; ++y)
   {
      std::vector<int> temp;
      for (uint32_t x = 0; x < this->width; ++x)
      {
         temp.push_back(0);
      }
      this->mazeArray.push_back(temp);
   }



   this->startingPoint.first = 0;
   this->startingPoint.second = 0;

   this->endPoint.first = width - 1;
   this->endPoint.second = height - 1;
}


std::pair<int, int> Maze::getStartingPoint() const 
{
   return this->startingPoint;
}

std::pair<int, int> Maze::getEndPoint() const 
{
   return this->endPoint;
}

void Maze::finishedGenerating()
{
   this->generatingFinished = true;
}

bool Maze::isGeneratingFinished() const
{
   return this->generatingFinished;
}

uint32_t Maze::getHeight() const
{
   return this->height;
}

uint32_t Maze::getWidth() const
{
   return this->width;
}

uint32_t Maze::getMarkerSize() const
{
   return this->markerSize;
}

std::vector<Marker> Maze::getMarkers() const
{
   return this->markers;
}

void Maze::removeMarkers()
{
   this->markers.clear();
}

uint32_t Maze::getTileSize() const
{
   return this->tileSize;
}

uint32_t Maze::getPathWidth() const
{
   return this->pathWidth;
}


