#include "AntsManager.h"
#include <algorithm>
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
   this->antsMarkers.clear();
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
      std::vector<Marker> surrMarkers;
      for(const auto& marker : this->antsMarkers)
      {
         if((marker.getX() == ant.getX() && marker.getY() == ant.getY()) ||   //present pos
            (marker.getX() == ant.getX() + 1 && marker.getY() == ant.getY()) || //right
            (marker.getX() == ant.getX() - 1 && marker.getY() == ant.getY()) || //left
            (marker.getX() == ant.getX() && marker.getY() == ant.getY() + 1) || //down
            (marker.getX() == ant.getX() && marker.getY() == ant.getY() - 1)) //up
         {
            surrMarkers.push_back(marker);
         }
      }

      //update them
      auto optionalMarker = ant.update(this->maze->mazeArray[ant.getY()][ant.getX()], surrMarkers);

      if(optionalMarker)
      {
         /*const auto it = std::find_if(this->antsMarkers.begin(), this->antsMarkers.end(), [optionalMarker](const Marker& marker)
            {
               return optionalMarker->getPos() == marker.getPos();
            });

         if(it == this->antsMarkers.end())
         {
         }*/
            this->antsMarkers.push_back(*optionalMarker);
      }

      //temporary
      if(true == ant.isFinishedMaze())
      {
         emit antsFinishedMaze();
      }
   }
}

/*void AntsManager::deleteAntsMarker(int x, int y) 
{
   auto it = this->antsMarkers.begin();
   while (it != this->antsMarkers.end())
   {
      if (it->getX == x && it->getY == y) 
      {
         it = this->antsMarkers.erase(it);
      }
      else
      {
         ++it;
      }
   }
 //  std::remove(this->antsMarkers.begin(), this->antsMarkers.end(), std::pair<int, int>(x, y));
}*/

const std::vector<Ant>& AntsManager::getAnts() const
{
   return this->ants;
}

const std::vector<Marker>& AntsManager::getAntsMarkers() const
{
   return this->antsMarkers;
}
