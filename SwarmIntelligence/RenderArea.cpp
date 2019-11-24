#include "RenderArea.h"
#include "ui_RenderAreaUI.h"
#include "LeeAlgorithm.h"
#include <iostream>
#include <QGraphicsRectItem>
#include <Maze.h>


RenderArea::RenderArea(QWidget* parent)
   : QGraphicsView(parent), ui(new Ui::RenderArea)
{
   ui->setupUi(this);

   this->scene = new QGraphicsScene(this);
   this->setScene(this->scene);


   this->wallColor = Qt::black;
   this->visitedTileColor = Qt::blue;
   this->shortestPathColor = Qt::green;
   this->notVisitedTileColor = Qt::white;
   this->startColor = Qt::blue;
   this->endColor = Qt::red;

}

RenderArea::~RenderArea()
{
   delete ui;
}

void RenderArea::addMazeToScene(const Maze& maze)
{
   this->scene->clear();

   //this->fitInView(this->scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);
   const auto& mazeArray = maze.mazeArray;
   const auto& pathWidth = maze.getPathWidth();
   const auto& mazeWidth = maze.getWidth();
   const auto& mazeHeight = maze.getHeight();

   const auto& tileSize = maze.getTileSize();

   //draw border
   createTile(0, 0, tileSize, tileSize * mazeHeight * (pathWidth + 1), this->wallColor);
   createTile(tileSize * mazeWidth * (pathWidth + 1), 0, tileSize, tileSize * mazeHeight * (pathWidth + 1), this->wallColor);

   createTile(0, 0, tileSize * mazeWidth * (pathWidth + 1), tileSize, this->wallColor);
   createTile(0, tileSize * mazeHeight * (pathWidth + 1), tileSize * mazeWidth * (pathWidth + 1) + tileSize, tileSize, this->wallColor);


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

         if((tile & Maze::CELL_END) == Maze::CELL_END)
         {
            color = this->endColor;
         }
         else if((tile & Maze::CELL_START) == Maze::CELL_START)
         {
            color = this->startColor;
         }

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

   for(const auto& m : maze.getMarkers())
   {
      createMarker(maze, m);
   }

   this->update();
}

void RenderArea::clear()
{
   this->scene->clear();
}

bool RenderArea::saveScreenshot(const QString& filePath)
{
   QImage img(this->scene->sceneRect().size().toSize(), QImage::Format_RGB32);
   QPainter painter(&img);
   this->scene->render(&painter);

   return img.save(filePath);
}

void RenderArea::createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor)
{
   auto tile = new QGraphicsRectItem;
   tile->setRect(x, y, tileWidth, tileHeight);
   tile->setBrush(tileColor);
   tile->setPen(noPen);
   this->scene->addItem(tile);
}

void RenderArea::createMarker(const Maze& maze, const Marker& marker)
{
   const double x = (maze.getPathWidth()) / 2. * maze.getTileSize() + marker.getX() * (maze.getPathWidth() + 1) * maze.getTileSize();
   const double y = (maze.getPathWidth()) / 2. * maze.getTileSize() + marker.getY() * (maze.getPathWidth() + 1) * maze.getTileSize();

   auto markerTile = new QGraphicsEllipseItem(x, y, maze.getMarkerSize(), maze.getMarkerSize());
   markerTile->setBrush(marker.getColor());
   markerTile->setPen(noPen);
   this->scene->addItem(markerTile);
}

std::vector<std::vector<int>> RenderArea::createShortestPath(const Maze& maze)
{
   LeeAlgorithm lee(maze);
   lee.solveMaze();

   const auto& pathWidth = maze.getPathWidth();
   const auto& mazeWidth = maze.getWidth();
   const auto& mazeHeight = maze.getHeight();

   auto tileSize = maze.getTileSize();

   std::vector<std::vector<int>> leeArray = lee.getArrayToDraw();

   for(uint32_t y = 0; y < mazeHeight; ++y)
   {
      for(uint32_t x = 0; x < mazeWidth; ++x)
      {
         auto tile = leeArray[y][x];
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
   return lee.getArrayToDraw();
}
