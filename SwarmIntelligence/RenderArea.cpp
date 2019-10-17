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

   for (int x = 0; x < mazeWidth; ++x)
   {
      //********* - draw upper line
      for (int i = 0; i < mazeWidth * (pathWidth + 1) + 1; ++i)
      {
         auto item = new QGraphicsRectItem;
         item->setRect(i * maze.getTileWidth(), 
                       x*(pathWidth + 1)*maze.getTileHeight(), 
                       maze.getTileWidth(), maze.getTileHeight());
         item->setBrush(Qt::black);
         item->setPen(QPen(Qt::black));
         this->scene->addItem(item);
      }
      for (int y = 0; y < mazeHeight; ++y)
      {
         //*000
         //*000
         //*000

         for (int px = 0; px < pathWidth; ++px)
         {
            for (int py = 0; py < pathWidth; ++py)
            {
               //*
               if (py == 0)
               {
                  auto item2 = new QGraphicsRectItem;
                  item2->setRect(y * (pathWidth + 1) * maze.getTileWidth(), 
                                (px + 1) * maze.getTileHeight() + x * (pathWidth + 1) * maze.getTileHeight(),
                                maze.getTileWidth(), maze.getTileHeight());
                  if (mazeArray[x][y] & Maze::CELL_PATH_N) {
                     item2->setBrush(Qt::white);
                     item2->setPen(QPen(Qt::white));
                  }
                  else {
                     item2->setBrush(Qt::black);
                     item2->setPen(QPen(Qt::black));
                  }
                  
                  this->scene->addItem(item2);
               }
               //000
               auto item = new QGraphicsRectItem;
               item->setRect((py + 1) * maze.getTileWidth() + y * (pathWidth + 1) * maze.getTileWidth(),
                  (px + 1) * maze.getTileHeight() + x * (pathWidth + 1) * maze.getTileHeight(),
                  maze.getTileWidth(), maze.getTileHeight());
               if (mazeArray[x][y] & Maze::CELL_VISITED)
               {
                  
                  item->setBrush(Qt::white);
                  item->setPen(QPen(Qt::white));
               }
               else
               {
                  item->setBrush(Qt::blue);
                  item->setPen(QPen(Qt::blue));
               }
               this->scene->addItem(item);
            }
         }
          //it cover the upper line of single cell with white block
         if (mazeArray[x][y] & Maze::CELL_PATH_W)
         {
            auto item2 = new QGraphicsRectItem;
            item2->setRect((y * (pathWidth + 1) + 1) * maze.getTileWidth(),
               x * (pathWidth + 1) * maze.getTileHeight(),
               pathWidth * maze.getTileWidth() - 1,
               maze.getTileHeight());
            item2->setBrush(Qt::white);
            item2->setPen(QPen(Qt::white));
            this->scene->addItem(item2);
         }
      }
      //draw last frame in every line, like:  *000*000*000 ---> this thing *
      for (int px = 0; px < pathWidth; ++px)
      {
         auto item2 = new QGraphicsRectItem;
         item2->setRect(mazeWidth * maze.getTileWidth() * (pathWidth + 1),
            x * (pathWidth + 1) * maze.getTileHeight() + (px + 1) * maze.getTileHeight(),
            maze.getTileWidth(), maze.getTileHeight());
         item2->setBrush(Qt::black);
         item2->setPen(QPen(Qt::black));
         this->scene->addItem(item2);
      }
   }
   //draw last line
   for (int i = 0; i < mazeWidth * (pathWidth + 1) + 1; ++i)
   {
      auto item = new QGraphicsRectItem;
      item->setRect(i * maze.getTileWidth(),
         mazeHeight * (pathWidth + 1) * maze.getTileHeight(), 
         maze.getTileWidth(), maze.getTileHeight());
      item->setBrush(Qt::black);
      item->setPen(QPen(Qt::black));
      this->scene->addItem(item);
   }
   this->update();
}
