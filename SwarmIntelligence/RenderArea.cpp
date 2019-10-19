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
   auto mazeArray = maze.getMazeArray();
   auto pathWidth = maze.getPathWidth();
   auto mazeWidth = maze.getWidth();
   auto mazeHeight = maze.getHeight();

   auto tileWidth = maze.getTileWidth();
   auto tileHeight = maze.getTileHeight();

   for (uint32_t x = 0; x < mazeWidth; ++x)
   {
      //********* - draw upper line (upper border)
      for (uint32_t i = 0; i < mazeWidth * (pathWidth + 1) + 1; ++i)
      {
         createTile(i * tileWidth,x * (pathWidth + 1) * tileHeight, tileWidth, tileHeight, this->wallColor);
      }

      for (uint32_t y = 0; y < mazeHeight; ++y)
      {
         //*000
         //*000
         //*000

         for (uint32_t px = 0; px < pathWidth; ++px)
         {
            for (uint32_t py = 0; py < pathWidth; ++py)
            {
               //*
               if (py == 0)
               {
                  QColor color;
                  if (mazeArray[x][y] & Maze::CELL_PATH_N)
                  {
                     color = this->notVisitedTileColor;
                  }
                  else
                  {
                     color = this->wallColor;
                  }

                  createTile(y * (pathWidth + 1) * tileWidth, (px + 1) * tileWidth + x * (pathWidth + 1) * tileHeight, tileWidth, tileHeight, color);
               }
               //000
               QColor color;
               if (mazeArray[x][y] & Maze::CELL_VISITED)
               {
                  color = this->notVisitedTileColor;
               }
               else
               {
                  color = this->visitedTileColor;
               }
               createTile((py + 1) * tileWidth + y * (pathWidth + 1) * tileWidth, (px + 1) * tileHeight + x * (pathWidth + 1) * tileHeight,
                  tileWidth, tileHeight, color);
            }
         }
         //it cover the upper line of single cell with white block
         if (mazeArray[x][y] & Maze::CELL_PATH_W)
         {
            createTile((y * (pathWidth + 1) + 1) * tileWidth, x * (pathWidth + 1) * tileHeight,
               pathWidth * tileWidth - 1, tileHeight, this->notVisitedTileColor);
         }
      }
      //draw last frame in every line, like:  *000*000*000 ---> this thing *
      uint32_t leftBorderLinePosX = mazeWidth * tileWidth * (pathWidth + 1);
      for (uint32_t px = 0; px < pathWidth; ++px)
      {
         //sprawdzic dwa tileHeight'y
         createTile(leftBorderLinePosX, x * (pathWidth + 1) * tileHeight + (px + 1) * tileHeight, tileWidth, tileHeight, this->wallColor);
      }
   }

   //draw last line (bottom border)
   uint32_t bottomBorderPosY = mazeHeight * (pathWidth + 1) * tileHeight;
   for (uint32_t i = 0; i < mazeWidth * (pathWidth + 1) + 1; ++i)
   {
      createTile(i * tileWidth, bottomBorderPosY, tileWidth, tileHeight, this->wallColor);
   }
   this->update();
}

void RenderArea::createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor)
{
   auto tile = new QGraphicsRectItem;
   tile->setRect(x, y, tileWidth, tileHeight);
   tile->setBrush(tileColor);
   tile->setPen(QPen(tileColor));
   this->scene->addItem(tile);

}
