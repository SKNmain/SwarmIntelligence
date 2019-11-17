#include "Maze.h"
#include <ctime>

Maze::Maze(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth, uint32_t markerSize)
{
   this->width = width;
   this->height = height;

   this->markerSize = markerSize;
   this->tileSize = tileSize;
   this->pathWidth = pathWidth;
   srand(time(0));

   for (uint32_t y = 0; y < this->height; ++y)
   {
      std::vector<int> temp;
      for (uint32_t x = 0; x < this->width; ++x)
      {
         temp.push_back(0);
      }
      this->mazeArray.push_back(temp);
   }

   stack.push(std::make_pair(0, 0));
   this->mazeArray[0][0] = Maze::CELL_VISITED;
   this->numberOfVisitedCells++;

   this->startingPoint.first = 0;
   this->startingPoint.second = 0;

   this->endPoint.first = width - 1;
   this->endPoint.second = height - 1;
}

void Maze::generateStep()
{
   if (numberOfVisitedCells < this->getHeight() * this->getWidth())
   {
      //Establish unvisited neighbours
      std::vector<int> neighbours;

      uint32_t x = stack.top().first;
      uint32_t y = stack.top().second;

      //North
      if (y > 0)
      {
         if ((mazeArray[y - 1][x] & CELL_VISITED) == 0)
         {
            neighbours.push_back(0);
         }
      }
      //South
      if (y < this->getHeight() - 1)
      {
         if ((mazeArray[y + 1][x] & CELL_VISITED) == 0)
         {
            neighbours.push_back(2);
         }
      }
      //West
      if (x > 0)
      {
         if ((mazeArray[y][x - 1] & CELL_VISITED) == 0)
         {
            neighbours.push_back(3);
         }
      }
      //East
      if (x < this->getWidth() - 1)
      {
         if ((mazeArray[y][x + 1] & CELL_VISITED) == 0)
         {
            neighbours.push_back(1);
         }
      }

      if (!neighbours.empty())
      {
         int next_cell_dir = neighbours[rand() % neighbours.size()];
         //create path
         switch (next_cell_dir)
         {
         case 0: //North
            mazeArray[y][x] |= CELL_PATH_N;
            mazeArray[y - 1][x] |= CELL_PATH_S | CELL_VISITED;
            stack.push(std::make_pair((x + 0), (y - 1)));
            break;
         case 1: //East
            mazeArray[y][x] |= CELL_PATH_E;
            mazeArray[y][x + 1] |= CELL_PATH_W | CELL_VISITED;
            stack.push(std::make_pair((x + 1), (y)));
            break;
         case 2: //South
            mazeArray[y][x] |= CELL_PATH_S;
            mazeArray[y + 1][x] |= CELL_PATH_N | CELL_VISITED;
            stack.push(std::make_pair((x + 0), (y + 1)));
            break;
         case 3:
            mazeArray[y][x] |= CELL_PATH_W;
            mazeArray[y][x - 1] |= CELL_PATH_E | CELL_VISITED;
            stack.push(std::make_pair((x - 1), (y)));
            break;
         }
         numberOfVisitedCells++;

         markers.push_back(Marker( stack.top().first, stack.top().second ));
      }
      else {
         //pop out of stack
         markers.push_back(Marker(stack.top().first, stack.top().second));
         stack.pop();
      }
   }
   else
   {
      isMazeDone = true;
   }
}

void Maze::generateMaze()
{
   this->markers.clear();

   while (false == isMazeDone)
   {
      generateStep();
   }
}

std::pair<int, int> Maze::getStartingPoint() const 
{
   return this->startingPoint;
}

std::pair<int, int> Maze::getEndPoint() const 
{
   return this->endPoint;
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

std::vector<std::vector<int>> Maze::getMazeArray() const
{
   return this->mazeArray;
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


