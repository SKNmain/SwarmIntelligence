#include "Ant.h"
#include "Maze.h"

RandGen<std::knuth_b>Ant::randGen = RandGen<std::knuth_b>(time(NULL)); 

Ant::Ant(int _id, int _x, int _y)
   : id(_id), x(_x), y(_y)
{
}

void Ant::update(int tile)
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

   int i = randGen.rand(0, road.size() - 1);
   tile = road[i];

   if(Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
   {
      y += -1;
   }
   if(Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
   {
      y += 1;
   }
   if(Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
   {
      x += 1;
   }
   if(Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
   {
      x += -1;
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
