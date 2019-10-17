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

   for (int x = 0; x < this->width; ++x)
   {
      std::vector<int> temp;
      for (int y = 0; y < this->height; ++y) {
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
   while (numberOfVisitedCells < this->getHeight() * this->getWidth())
   {
      //Establish unvisited neighbours
      std::vector<int> neighbours;
      //North
      if (stack.top().second > 0) {
         if ((mazeArray[stack.top().first][stack.top().second - 1] & CELL_VISITED) == 0) {
            neighbours.push_back(0);
         }
      }
      //East
      if (stack.top().first < this->getWidth() - 1) {
         if ((mazeArray[stack.top().first + 1][stack.top().second] & CELL_VISITED) == 0) {
            neighbours.push_back(1);
         }
      }
      //South
      if (stack.top().second < this->getWidth() - 1) {
         if ((mazeArray[stack.top().first][stack.top().second + 1] & CELL_VISITED) == 0) {
            neighbours.push_back(2);
         }
      }
      //West
      if (stack.top().first > 0) {
         if ((mazeArray[stack.top().first - 1][stack.top().second] & CELL_VISITED) == 0) {
            neighbours.push_back(3);
         }
      }

      if (!neighbours.empty()) {
         int next_cell_dir = neighbours[rand() % neighbours.size()];
         //create path
         switch (next_cell_dir) {
         case 0: //North
            mazeArray[stack.top().first][stack.top().second] |= CELL_PATH_N;
            mazeArray[stack.top().first][stack.top().second - 1] |= CELL_PATH_S | CELL_VISITED;
            stack.push(std::make_pair((stack.top().first + 0), (stack.top().second - 1)));
            break;
         case 1: //East
            mazeArray[stack.top().first][stack.top().second] |= CELL_PATH_E;
            mazeArray[stack.top().first + 1][stack.top().second] |= CELL_PATH_W | CELL_VISITED;;
            stack.push(std::make_pair((stack.top().first + 1), (stack.top().second)));
            break;
         case 2: //South
            mazeArray[stack.top().first][stack.top().second] |= CELL_PATH_S;
            mazeArray[stack.top().first][stack.top().second + 1] |= CELL_PATH_N | CELL_VISITED;;
            stack.push(std::make_pair((stack.top().first + 0), (stack.top().second + 1)));
            break;
         case 3:
            mazeArray[stack.top().first][stack.top().second] |= CELL_PATH_W;
            mazeArray[stack.top().first - 1][stack.top().second] |= CELL_PATH_E | CELL_VISITED;;
            stack.push(std::make_pair((stack.top().first - 1), (stack.top().second)));
            break;
         }
         numberOfVisitedCells++;
      }
      else {
         //pop out of stack
         stack.pop();
      }
   }
   //else {
     // isMazeDone = true;
   //}
   
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


