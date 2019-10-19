#pragma once
#include <QGraphicsView>
#include <QPen>
#include <QBrush>

class Maze;

namespace Ui {
   class RenderArea;
}

class RenderArea : public QGraphicsView
{
   Q_OBJECT;

public:
   RenderArea(QWidget* parent = nullptr);
   ~RenderArea();

   void addMazeToScene(const Maze& maze);

private:
   void createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor);


   QColor wallColor;
   QColor notVisitedTileColor;
   QColor visitedTileColor;


   Ui::RenderArea* ui;
   std::vector<QGraphicsItem*> sceneElements;
   QGraphicsScene* scene;
};

