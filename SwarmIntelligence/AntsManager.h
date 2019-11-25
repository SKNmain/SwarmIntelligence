#pragma once
#include "Ant.h"
#include <vector>

class Maze;

class AntsManager
{
public:
   AntsManager() = default;

   void createAnt(int id, int x = 0, int y = 0);

   void initialize(const Maze* maze);
   void step();

   const std::vector<Ant>& getAnts() const;
private:
   std::vector<Ant> ants;
   const Maze* maze = nullptr;
};

