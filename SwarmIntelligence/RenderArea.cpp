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

   auto tileWidth = maze.getTileWidth();
   auto tileHeight = maze.getTileHeight();

   for (uint32_t y = 0; y < mazeHeight; ++y)
   {
      //draw horizontal line between layers of tiles
      createTile(0, y * tileHeight * (pathWidth + 1), tileWidth * mazeWidth * (pathWidth + 1), tileHeight, this->wallColor);

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
               createTile(px * tileWidth + x * (pathWidth + 1) * tileWidth, py * tileWidth + y * (pathWidth + 1) * tileHeight,
                  tileWidth, tileHeight, color);
            }
         }

         // *000
         // *000
         if (x != 0)
         {
            if (0 == (tile & Maze::CELL_PATH_W))
            {
               createTile(x * tileWidth * (pathWidth + 1), tileWidth + y * tileHeight * (pathWidth + 1),
                  tileWidth, tileHeight * (pathWidth), this->wallColor);
            }
            else
            {
               createTile(x * tileWidth * (pathWidth + 1), tileWidth + y * tileHeight * (pathWidth + 1),
                  tileWidth, tileHeight * (pathWidth), color);
            }
         }

         // ***
         // 000
         if (y != 0 && 0 != (tile & Maze::CELL_PATH_N))
         {
            createTile(x * tileWidth * (pathWidth + 1) + tileWidth, y * tileHeight * (pathWidth + 1),
               tileWidth * (pathWidth), tileHeight, color);
         }

      }
   }

   //draw border
   createTile(0, 0, tileWidth, tileHeight * mazeHeight * (pathWidth + 1), this->wallColor);
   createTile(tileWidth * mazeWidth * (pathWidth + 1), 0, tileWidth, tileHeight * mazeHeight * (pathWidth + 1), this->wallColor);

   createTile(0, 0, tileWidth * mazeWidth * (pathWidth + 1), tileHeight, this->wallColor);
   createTile(0, tileHeight * mazeHeight * (pathWidth + 1), tileWidth * mazeWidth * (pathWidth + 1) + tileWidth, tileHeight, this->wallColor);

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
   const double x = (maze.getPathWidth() ) / 2. * maze.getTileWidth() + marker.getX() * (maze.getPathWidth() + 1) * maze.getTileWidth();
   const double y = (maze.getPathWidth() ) / 2. * maze.getTileHeight() + marker.getY() * (maze.getPathWidth() + 1) * maze.getTileHeight();

   auto markerTile = new QGraphicsEllipseItem(x, y, maze.getMarkerSize(), maze.getMarkerSize());
   markerTile->setBrush(marker.getColor());
   markerTile->setPen(QPen(Qt::NoPen));
   this->scene->addItem(markerTile);
}
