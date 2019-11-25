#pragma once
#include <utility>
#include "RandGen.h"

class Ant
{
   friend class AntsManager;

public:
   Ant(int id, int x, int y);

   void update(int tile);

   int getID() const;
   int getX() const;
   int getY() const;
   std::pair<int, int> getPosition() const;

   bool isFinishedMaze() const;

private:
   static RandGen<std::knuth_b> randGen;

   bool finishedMaze = false;
   std::pair<int, int> lastPos;
   int id;
   int x = 0;
   int y = 0;
};

