#pragma once
#include <QDialog>
#include <QSettings>

class QSpinBox;
class QCheckBox;

namespace Ui {
   class AppSettings;
}

class AppSettings : public QDialog
{
   Q_OBJECT


public:
   explicit AppSettings(QWidget* parent = nullptr);
   ~AppSettings();

   int getMazeWidth() const;
   int getMazeHeight() const;
   int getMarkerSize() const;
   int getPathSize() const;
   int getTileSize() const;
   int getAnimationTime() const;

   bool isAnimationEnabled() const;
   bool isVisualize() const;

   void setMazeWidth(int val);
   void setMazeHeight(int val);
   void setMarkerSize(int val);
   void setPathSize(int val);
   void setTileSize(int val);
   void setAnimationTime(int val);

   void setAnimationEnabled(bool val);
   void setVisualizeEnabled(bool val);

private slots:
   void on_spinBox_markerSize_valueChanged(int val);
   void on_spinBox_pathSize_valueChanged(int val);
   void on_spinBox_tileSize_valueChanged(int val);
   void on_spinBox_mazeHeight_valueChanged(int val);
   void on_spinBox_mazeWidth_valueChanged(int val);

   void on_spinBox_animationTime_valueChanged(int val);

   void on_checkBox_animationEnabled_stateChanged(int state);

   void on_checkBox_visualize_stateChanged(int arg1);

private:
   void setStartingValue(const QString& optName, int val, QSpinBox* widget);
   void setStartingValue(const QString& optName, bool val, QCheckBox* widget);
   

   Ui::AppSettings* ui;
   QSettings settings{ "config.ini", QSettings::IniFormat };



   const char* MAZE_WIDTH_TAG = "MazeWidth";
   const char* MAZE_HEIGHT_TAG = "MazeHeight";
   const char* TILE_SIZE_TAG = "TileSize";
   const char* MARKER_SIZE_TAG = "MarkerSize";
   const char* PATH_SIZE_TAG = "PathSize";
   const char* ANIMATION_ENABLED_TAG = "AnimEnabled";
   const char* ANIMATION_TIME_TAG = "AnimTime";
   const char* VISUALIZE_TAG = "Visualize";
};

