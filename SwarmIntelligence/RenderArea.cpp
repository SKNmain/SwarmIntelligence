#include "RenderArea.h"
#include "ui_RenderAreaUI.h"
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
   this->notVisitedTileColor = Qt::white;
}

RenderArea::~RenderArea()
{
   delete ui;
}

void RenderArea::addMazeToScene(const Maze& maze)
{
   this->scene->clear();

   auto mazeArray = maze.getMazeArray();
   auto pathWidth = maze.getPathWidth();
   auto mazeWidth = maze.getWidth();
   auto mazeHeight = maze.getHeight();

   auto tileSize = maze.getTileSize();

   for (uint32_t y = 0; y < mazeHeight; ++y)
   {
      //draw horizontal line between layers of tiles
      createTile(0, y * tileSize * (pathWidth + 1), tileSize * mazeWidth * (pathWidth + 1), tileSize, this->wallColor);

      for (uint32_t x = 0; x < mazeWidth; ++x)
      {
         auto tile = mazeArray[y][x];

         //visited or not?
         QColor color;
         if (tile & Maze::CELL_VISITED)
         {
            color = this->notVisitedTileColor;
         }
         else
         {
            color = this->visitedTileColor;
         }

         //draw path
         for (uint32_t py = 1; py <= pathWidth; ++py)
         {
            for (uint32_t px = 1; px <= pathWidth; ++px)
            {
               createTile(px * tileSize + x * (pathWidth + 1) * tileSize, py * tileSize + y * (pathWidth + 1) * tileSize,
                  tileSize, tileSize, color);
            }
         }

         // *000
         // *000
         if (x != 0)
         {
            if (0 == (tile & Maze::CELL_PATH_W))
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
         if (y != 0 && 0 != (tile & Maze::CELL_PATH_N))
         {
            createTile(x * tileSize * (pathWidth + 1) + tileSize, y * tileSize * (pathWidth + 1),
               tileSize * (pathWidth), tileSize, color);
         }

      }
   }

   //draw border
   createTile(0, 0, tileSize, tileSize * mazeHeight * (pathWidth + 1), this->wallColor);
   createTile(tileSize * mazeWidth * (pathWidth + 1), 0, tileSize, tileSize * mazeHeight * (pathWidth + 1), this->wallColor);

   createTile(0, 0, tileSize * mazeWidth * (pathWidth + 1), tileSize, this->wallColor);
   createTile(0, tileSize * mazeHeight * (pathWidth + 1), tileSize * mazeWidth * (pathWidth + 1) + tileSize, tileSize, this->wallColor);

   for(const auto& m : maze.getMarkers())
   {
      createMarker(maze, m);
   }

   this->update();
}

void RenderArea::createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor)
{
   auto tile = new QGraphicsRectItem;
   tile->setRect(x, y, tileWidth, tileHeight);
   tile->setBrush(tileColor);
   tile->setPen(QPen(Qt::NoPen));
   this->scene->addItem(tile);
}

void RenderArea::createMarker(const Maze& maze, const Marker& marker)
{
   const double x = (maze.getPathWidth() ) / 2. * maze.getTileSize() + marker.getX() * (maze.getPathWidth() + 1) * maze.getTileSize();
   const double y = (maze.getPathWidth() ) / 2. * maze.getTileSize() + marker.getY() * (maze.getPathWidth() + 1) * maze.getTileSize();

   auto markerTile = new QGraphicsEllipseItem(x, y, maze.getMarkerSize(), maze.getMarkerSize());
   markerTile->setBrush(marker.getColor());
   markerTile->setPen(QPen(Qt::NoPen));
   this->scene->addItem(markerTile);
}
