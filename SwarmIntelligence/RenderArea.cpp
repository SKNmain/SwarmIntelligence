#include "RenderArea.h"
#include "ui_RenderAreaUI.h"
#include "LeeAlgorithm.h"
#include <iostream>
#include <QGraphicsRectItem>
#include <Maze.h>
#include <QDebug>
#include "AntsManager.h"
#include "AppSettings.h"


RenderArea::RenderArea(QWidget* parent)
   : QGraphicsView(parent), ui(new Ui::RenderArea)
{
   ui->setupUi(this);

   setDragMode(QGraphicsView::ScrollHandDrag);

   this->scene = new QGraphicsScene(this);
   this->setScene(this->scene);


   this->wallColor = Qt::black;
   this->visitedTileColor = Qt::blue;
   this->shortestPathColor = Qt::green;
   this->notVisitedTileColor = Qt::white;
   this->startColor = Qt::blue;
   this->endColor = Qt::red;
   this->antColor = Qt::green;
}

RenderArea::~RenderArea()
{
   delete ui;
}

void RenderArea::initSettings(const AppSettings* settings)
{
   this->sett = settings;
}

void RenderArea::addMazeToScene(const Maze& maze)
{
   clear();

   //this->fitInView(this->scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);
   const auto& mazeArray = maze.mazeArray;
   const auto& pathWidth = this->sett->getPathSize();
   const auto& mazeWidth = this->sett->getMazeWidth();
   const auto& mazeHeight = this->sett->getMazeHeight();

   const auto& tileSize = this->sett->getTileSize();

   //draw border
   createTile(0, 0, tileSize, tileSize * mazeHeight * (pathWidth + 1), this->wallColor);
   createTile(tileSize * mazeWidth * (pathWidth + 1), 0, tileSize, tileSize * mazeHeight * (pathWidth + 1), this->wallColor);

   createTile(0, 0, tileSize * mazeWidth * (pathWidth + 1), tileSize, this->wallColor);
   createTile(0, tileSize * mazeHeight * (pathWidth + 1), tileSize * mazeWidth * (pathWidth + 1) + tileSize, tileSize, this->wallColor);

   createTile(0, 1, tileSize, tileSize * pathWidth + 1, this->startColor);
   createTile((mazeWidth - 1) * (pathWidth + 1) * tileSize + tileSize,
      mazeHeight * (pathWidth + 1) * tileSize,
      tileSize * pathWidth + 1,
      tileSize, this->endColor);


   for(uint32_t y = 0; y < mazeHeight; ++y)
   {
      //draw horizontal line between layers of tiles
      createTile(0, y * tileSize * (pathWidth + 1), tileSize * mazeWidth * (pathWidth + 1), tileSize, this->wallColor);

      for(uint32_t x = 0; x < mazeWidth; ++x)
      {
         auto tile = mazeArray[y][x];

         //visited or not?
         QColor color;
         if(tile & Maze::CELL_VISITED)
         {
            color = this->notVisitedTileColor;
         }
         else
         {
            color = this->visitedTileColor;
         }

         /*if((tile & Maze::CELL_END) == Maze::CELL_END)
         {
            color = this->endColor;
         }
         else if((tile & Maze::CELL_START) == Maze::CELL_START)
         {
            color = this->startColor;
         }*/

         //draw path
         for(uint32_t py = 1; py <= pathWidth; ++py)
         {
            for(uint32_t px = 1; px <= pathWidth; ++px)
            {
               createTile(px * tileSize + x * (pathWidth + 1) * tileSize, py * tileSize + y * (pathWidth + 1) * tileSize,
                  tileSize, tileSize, color);
            }
         }

         // *000
         // *000
         if(x != 0)
         {
            if(0 == (tile & Maze::CELL_PATH_W))
            {
               createTile(x * tileSize * (pathWidth + 1), tileSize + y * tileSize * (pathWidth + 1),
                  tileSize, tileSize * (pathWidth), this->wallColor);
            }
            else
            {
               createTile(x * tileSize * (pathWidth + 1), tileSize + y * tileSize * (pathWidth + 1),
                  tileSize, tileSize * (pathWidth), color);
            }
         }

         // ***
         // 000
         if(y != 0 && 0 != (tile & Maze::CELL_PATH_N))
         {
            createTile(x * tileSize * (pathWidth + 1) + tileSize, y * tileSize * (pathWidth + 1),
               tileSize * (pathWidth), tileSize, color);
         }



      }
   }
   createMarker(maze.lastPostWhileMazeGen);

   this->update();
}

void RenderArea::clear()
{
   this->scene->clear();
   this->antsGraphics.clear();
}

bool RenderArea::saveScreenshot(const QString& filePath)
{
   QImage img(this->scene->sceneRect().size().toSize(), QImage::Format_RGB32);
   QPainter painter(&img);
   this->scene->render(&painter);

   return img.save(filePath);
}

void RenderArea::drawShortestPath(const Maze* maze)
{
   const auto& tileSize = maze->getTileSize();
   const auto& pathWidth = maze->getPathWidth();
   const auto& mazeWidth = maze->getWidth();
   const auto& mazeHeight = maze->getHeight();

   for(uint32_t y = 0; y < mazeHeight; ++y)
   {
      for(uint32_t x = 0; x < mazeWidth; ++x)
      {
         auto tile = maze->shortestWayArray[y][x];
         QColor color = this->shortestPathColor;

         if(Maze::CELL_SHORTEST == (tile & Maze::CELL_SHORTEST))
         {
            for(uint32_t py = 1; py <= pathWidth; ++py)
            {
               for(uint32_t px = 1; px <= pathWidth; ++px)
               {
                  createTile(px * tileSize + x * (pathWidth + 1) * tileSize, py * tileSize + y * (pathWidth + 1) * tileSize,
                     tileSize, tileSize, color);
               }
            }
         }
      }
   }
   this->update();
}

void RenderArea::renderAnts(const AntsManager& antsManager)
{
   drawAntsMarkers(antsManager.getAntsMarkers());
   drawAnts(antsManager);
}

void RenderArea::clearAntsMarkersFromScene()
{
   for(auto& i : this->markersGraphics)
   {
      this->scene->removeItem(i);
   }
   this->markersGraphics.clear();
}

void RenderArea::drawAntsMarkers(const std::vector<Marker>& antsMarkers)
{
   clearAntsMarkersFromScene();

   for(const auto& m: antsMarkers)
   {
      if (m.getType() == Marker::NOT_FULLY_DISCOVER_PATH)
      {
         createMarker(m);
      }
      
   }
   for (const auto& m : antsMarkers)
   {
      if (m.getType() == Marker::CLOSED_PATH)
      {
         createMarker(m);
      }

   }
   for (const auto& m : antsMarkers)
   {
      if (m.getType() == Marker::PATH_TO_EXIT)
      {
         createMarker(m);
      }

   }
}

void RenderArea::drawAnts(const AntsManager& antsManager)
{
   //clear
   for(auto& i : this->antsGraphics)
   {
      this->scene->removeItem(i);
   }
   this->antsGraphics.clear();

   //map ants to their position, (to simplify display on the same position)
   std::map<std::pair<int, int>, std::vector<const Ant*>> visitedPosition;
   for(const auto& ant : antsManager.getAnts())
   {
      visitedPosition[ant.getPosition()].push_back(&ant);
   }

   for(const auto& [pos, ants] : visitedPosition)
   {
      double offset = (this->sett->getTileSize() * this->sett->getPathSize() - this->sett->getAntSize());
      offset = offset != 0 ? offset / 2.f : offset;
      const double x = pos.first * (this->sett->getPathSize()) * this->sett->getTileSize() + this->sett->getTileSize() * (1 + pos.first) + offset;
      const double y = pos.second * (this->sett->getPathSize()) * this->sett->getTileSize() + this->sett->getTileSize() * (1 + pos.second) + offset;

      auto item = new QGraphicsEllipseItem(x, y, this->sett->getAntSize(), this->sett->getAntSize());
      item->setBrush(this->sett->getAntsColor());
      auto text = new QGraphicsTextItem(item);
      //one ant on tile
      if(ants.size() == 1)
      {
         text->setPlainText("ID:" + QString::number(ants[0]->getID()));
      }
      else // many 
      {
         text->setPlainText("C:" + QString::number(ants.size()));

      }
      text->setPos({ x, y });
      this->scene->addItem(item);
      this->antsGraphics.push_back(item);
   }
}

void RenderArea::createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor)
{
   auto tile = new QGraphicsRectItem;
   tile->setRect(x, y, tileWidth, tileHeight);
   tile->setBrush(tileColor);
   tile->setPen(noPen);
   this->scene->addItem(tile);
}

void RenderArea::createMarker(const Marker& marker)
{
   double offset = (this->sett->getTileSize() * this->sett->getPathSize() - this->sett->getMarkerSize());
   offset = offset != 0 ? offset / 2.f : offset;
   const double x = marker.getX() * (this->sett->getPathSize()) * this->sett->getTileSize() + this->sett->getTileSize() * (1 + marker.getX()) + offset;
   const double y = marker.getY() * (this->sett->getPathSize()) * this->sett->getTileSize() + this->sett->getTileSize() * (1 + marker.getY()) + offset;

   auto markerTile = new QGraphicsEllipseItem(x, y, this->sett->getMarkerSize(), this->sett->getMarkerSize());
   markerTile->setBrush(marker.getColor());
   markerTile->setPen(noPen);
   this->scene->addItem(markerTile);
   this->markersGraphics.push_back(markerTile);
}

