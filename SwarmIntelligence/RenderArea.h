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
   Ui::RenderArea* ui;

   std::vector<QGraphicsItem*> sceneElements;
   QGraphicsScene* scene;
};

