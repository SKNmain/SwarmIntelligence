#pragma once
#include "Ant.h"
#include <vector>
#include <qobject.h>
#include "Marker.h"

class Maze;
class AppSettings;

class AntsManager : public QObject
{
   Q_OBJECT;

public:
   AntsManager(const AppSettings* appSettings);

   void createAnt(int id, int x = 0, int y = 0);

   void initialize(Maze* maze);
   void step();

   const std::vector<Ant>& getAnts() const;
   const std::vector<Marker>& getAntsMarkers() const;
   //void deleteAntsMarker(int x, int y);

signals:
   void antsFinishedMaze();
private:
   std::vector<Marker> antsMarkers;

   std::vector<Ant> ants;
   Maze* maze = nullptr;
   const AppSettings* appSettings = nullptr;
};

