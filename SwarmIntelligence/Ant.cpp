#include "Ant.h"
#include "Maze.h"

RandGen<std::knuth_b>Ant::randGen = RandGen<std::knuth_b>(time(NULL));

Ant::Ant(int _id, int _x, int _y)
   : id(_id), x(_x), y(_y), lastPos(_x, _y)
{
}

void Ant::update(int tile)
{
   if(false == this->finishedMaze)
   {

      std::vector<int> road;
      if(Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
      {
         road.push_back(Maze::CELL_PATH_N);
      }
      if(Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
      {
         road.push_back(Maze::CELL_PATH_S);
      }
      if(Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
      {
         road.push_back(Maze::CELL_PATH_E);
      }
      if(Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
      {
         road.push_back(Maze::CELL_PATH_W);
      }
      if(Maze::CELL_END == (tile & Maze::CELL_END))
      {
         this->finishedMaze = true;
         return;
      }

      int tempX = this->x;
      int tempY = this->y;
      bool safety = road.size() == 1;
      do
      {
         tempX = this->x;
         tempY = this->y;

         int i = randGen.rand(0, road.size() - 1);
         tile = road[i];

         if(Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
         {
            tempY += -1;
         }
         if(Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
         {
            tempY += 1;
         }
         if(Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
         {
            tempX += 1;
         }
         if(Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
         {
            tempX += -1;
         }


      } while(false == finishedMaze && false == safety && tempX == this->lastPos.first && tempY == this->lastPos.second);

      this->lastPos.first = this->x;
      this->lastPos.second = this->y;

      this->x = tempX;
      this->y = tempY;
   }
}

int Ant::getID() const
{
   return this->id;
}

int Ant::getX() const
{
   return this->x;
}

int Ant::getY() const
{
   return this->y;
}

std::pair<int, int> Ant::getPosition() const
{
   return { this->x, this->y };
}

bool Ant::isFinishedMaze() const
{
   return this->finishedMaze;
}
