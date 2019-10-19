#include "Maze.h"
#include <ctime>

Maze::Maze(uint32_t width, uint32_t height, uint32_t tileWidth, uint32_t tileHeight, uint32_t pathWidth)
{
   this->width = width;
   this->height = height;

   this->tileHeight = tileHeight;
   this->tileWidth = tileWidth;
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
}

void Maze::generateMaze()
{
   if(numberOfVisitedCells < this->getHeight() * this->getWidth())
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
      }
      else {
         //pop out of stack
         stack.pop();
      }
   }
   else {
      isMazeDone = true;
   }
}

uint32_t Maze::getHeight() const
{
   return this->height;
}

uint32_t Maze::getWidth() const
{
   return this->width;
}

std::vector<std::vector<int>> Maze::getMazeArray() const
{
   return this->mazeArray;
}

uint32_t Maze::getTileHeight() const
{
   return this->tileHeight;
}

uint32_t Maze::getTileWidth() const
{
   return this->tileWidth;
}

uint32_t Maze::getPathWidth() const
{
   return this->pathWidth;
}


