#include "AntsManager.h"
#include "Maze.h"
#include "Logger.hpp"
#include "AppSettings.h"

AntsManager::AntsManager(const AppSettings* appSettings)
{
   this->appSettings = appSettings;
}

void AntsManager::createAnt(int id, int x, int y)
{
   this->ants.push_back({ id, x, y });
}

void AntsManager::initialize(Maze* maze)
{
   //define here number of ants?
   //If we need, we should get from MazeSolver structure with additional info - how long is shortest path, number of empty path, etc, to analyze it, and decide number of ants
   //
   //
   //
   this->ants.clear();
   this->maze = maze;

   int numberOfAnts = 0;

   if(true == this->appSettings->isConstNumberOfAntsEnabled())
   {
      numberOfAnts = this->appSettings->getConstNumberOfAntsSize();
   }
   else
   {
      
   }

   for(int i = 0; i < numberOfAnts; ++i)
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


      //temporary
      if(true == ant.isFinishedMaze())
      {
         emit antsFinishedMaze();
      }
   }
}

const std::vector<Ant>& AntsManager::getAnts() const
{
   return this->ants;
}
