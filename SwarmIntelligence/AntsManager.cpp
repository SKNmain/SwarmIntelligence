#include "AntsManager.h"
#include "Maze.h"

void AntsManager::createAnt(int id, int x, int y)
{
   this->ants.push_back({ id, x, y });
}

void AntsManager::initialize(const Maze* maze)
{
   //define here number of ants?
   //If we need, we should get from MazeSolver structure with additional info - how long is shortest path, number of empty path, etc, to analyze it, and decide number of ants
   //
   //
   //
   this->maze = maze;

   this->ants.clear();
   for(int i = 0; i < 5; ++i)
   {
      createAnt(i, 0, 0);
   }
}

void AntsManager::step()
{
   for(auto& ant : this->ants)
   {
      //update them
      ant.update(this->maze->mazeArray[ant.getY()][ant.getX()]);
   }
}

const std::vector<Ant>& AntsManager::getAnts() const
{
   return this->ants;
}
