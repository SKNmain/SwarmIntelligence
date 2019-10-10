#include "RenderArea.h"
#include "ui_RenderAreaUI.h"

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
   for (int i = 0; i < mazeArray.size(); ++i)
   {
      for (int j = 0; j < mazeArray[i].size(); ++j)
      {
         auto tile = mazeArray[i][j];

         auto item = new QGraphicsRectItem;
         item->setRect(i * maze.getTileWidth(), j * maze.getTileHeight(), maze.getTileWidth(), maze.getTileHeight());
         switch (tile)
         {
         case Maze::MazeTileType::Wall:
         {
            item->setBrush(Qt::red);
            break;
         }
         case Maze::MazeTileType::Road:
         {
            item->setBrush(Qt::black);
            break;
         }
         case Maze::MazeTileType::Start: break;
         case Maze::MazeTileType::End: break;
         default:;
         }

         this->scene->addItem(item);
      }
   }

   this->update();
}
