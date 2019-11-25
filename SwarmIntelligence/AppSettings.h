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
   int getAntSize() const;
   int getConstNumberOfAntsSize() const;
   int getAntsAnimationTime() const;

   bool isAnimationEnabled() const;
   bool isVisualize() const;
   bool isGenerateMazeOnStart() const;
   bool isConstNumberOfAntsEnabled() const;

   QColor getAntsColor() const;
   void setAntColor(const QColor& color);

   void setMazeWidth(int val);
   void setMazeHeight(int val);
   void setMarkerSize(int val);
   void setPathSize(int val);
   void setTileSize(int val);
   void setAnimationTime(int val);
   void setAntsAnimationTime(int val);
   void setConstNumberOfAnts(int val);
   void setAntSize(int val);

   void setAnimationEnabled(bool val);
   void setConstNumberOfAnts(bool val);
   void setGenerateMazeOnStart(bool val);
   void setVisualizeEnabled(bool val);

private slots:
   void on_spinBox_markerSize_valueChanged(int val);
   void on_spinBox_pathSize_valueChanged(int val);
   void on_spinBox_tileSize_valueChanged(int val);
   void on_spinBox_mazeHeight_valueChanged(int val);
   void on_spinBox_mazeWidth_valueChanged(int val);
   void on_spinBox_animationTime_valueChanged(int val);
   void on_spinBox_antSize_valueChanged(int val);
   void on_spinBox_constNumberOfAnts_valueChanged(int val);
   void on_spinBox_antsAnimationTime_valueChanged(int val);

   void on_checkBox_animationEnabled_stateChanged(int state);
   void on_checkBox_visualize_stateChanged(int state);
   void on_checkBox_generateMazeOnStart_stateChanged(int state);
   void on_checkBox_isConstNumberOfAnts_stateChanged(int state);

   void on_pushButton_selectAntColor_clicked();




private:
   void setStartingValue(const QString& optName, int val, QSpinBox* widget);
   void setStartingValue(const QString& optName, bool val, QCheckBox* widget);
   void setStartingValue(const QString& optName, const QColor& color);
   

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
   const char* GENERATE_MAZE_ON_START_TAG = "GenMazeOnStart";
   const char* ANT_SIZE_TAG = "AntSize";
   const char* ANT_COLOR_TAG = "AntsColor";
   const char* ANT_ANIMATION_TIME_TAG = "AntsAnimationTime";
   const char* ANT_CONST_NUMBER_ENABLED_TAG = "ConstNumberAntsEnabled";
   const char* ANT_CONST_NUMBER_TAG = "ConstNumberAnts";
};

