#pragma once
#include <QGraphicsView>
#include <QPen>
#include <QBrush>
#include <QWheelEvent>

class AntsManager;
class Maze;
class Marker;
class AppSettings;

namespace Ui {
   class RenderArea;
}

class RenderArea : public QGraphicsView
{
   Q_OBJECT;

public:
   RenderArea(QWidget* parent = nullptr);
   ~RenderArea();

   void initSettings(const AppSettings* settings);

   void addMazeToScene(const Maze* maze);
   void clear();

   bool saveScreenshot(const QString& filePath);

   void drawShortestPath(const Maze* maze);

   void renderAnts(const AntsManager& antsManager);

   void clearAnts();
   void clearMarkers();
   void clearShortestWayTiles();
   void clearSceneElements();

   void setVisibleShortestWay(bool enable);
public slots:
   void wheelEvent(QWheelEvent* event)
   {
      if(event->delta() > 0)
         scale(1.25, 1.25);
      else
         scale(0.8, 0.8);
   }
private:
   void drawAnts(const AntsManager& antsManager);
   void drawAntsMarkers(const std::vector<Marker>& antsMarkers);
   QGraphicsRectItem* createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor);
   QGraphicsItem* createPathBlockedMarker(const Marker& marker, double offset, double x, double y);
   void createMarker(const Marker& marker);

   void cleanUpGraphicsItems(std::vector<QGraphicsItem*>& items);

   QColor wallColor;
   QColor notVisitedTileColor;
   QColor visitedTileColor;
   QColor shortestPathColor;
   QColor startColor;
   QColor endColor;
   QColor antColor;
   QPen noPen{ Qt::NoPen };

   Ui::RenderArea* ui;
   std::vector<QGraphicsItem*> sceneElements;
   std::vector<QGraphicsItem*> antsGraphics;
   std::vector<QGraphicsItem*> markersGraphics;
   std::vector<QGraphicsItem*> shortestWayTilesGraphics;
   QGraphicsScene* scene;

   const AppSettings* sett = nullptr;
};



