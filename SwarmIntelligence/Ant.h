#pragma once
#include <utility>
#include "RandGen.h"
#include "Marker.h"

class Ant
{
   friend class AntsManager;

public:
   Ant(int id, int x, int y);

   std::optional<Marker> update(int tile, const std::vector<Marker>& surrMarkers);

   int getID() const;
   int getX() const;
   int getY() const;
   std::pair<int, int> getPosition() const;

   bool isFinishedMaze() const;

private:
   static RandGen<std::knuth_b> randGen;
   bool changeNextYellowToRed = false;
   bool endFoundChangeToBlue = false;
   bool blueChangedGoToEnd = false;
   bool first = true;
   bool finishedMaze = false;
   std::pair<int, int> lastPos;
   std::pair<int, int> pos;
   int id;
};

