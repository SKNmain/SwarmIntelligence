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

   enum StartingMaze
   {
      LOAD_FROM_FILE,
      GENERATE_NEW,
      NO_STARTING
   };

   int getMazeWidth() const;
   int getMazeHeight() const;
   int getMarkerSize() const;
   int getPathSize() const;
   int getTileSize() const;
   int getAnimationTime() const;
   int getAntSize() const;
   int getConstNumberOfAntsSize() const;
   int getAntsAnimationTime() const;

   StartingMaze startingMazeStatus() const;
   QString pathToMaze() const;

   bool isAnimationEnabled() const;
   bool isVisualize() const;
   bool isConstNumberOfAntsEnabled() const;

   QColor getAntsColor() const;
   void setAntColor(const QColor& color);

   void setStartingMazeStatus(StartingMaze startingMaze);

   void setMazeWidth(int val);
   void setMazeHeight(int val);
   void setMarkerSize(int val);
   void setPathSize(int val);
   void setTileSize(int val);
   void setAnimationTime(int val);
   void setAntsAnimationTime(int val);
   void setNumberOfAnts(int val);
   void setAntSize(int val);

   void setAnimationEnabled(bool val);
   void setConstNumberOfAnts(bool val);
   void setVisualizeEnabled(bool val);

   void setLastMazePath(const QString& path);

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
   void on_checkBox_isConstNumberOfAnts_stateChanged(int state);

   void on_pushButton_selectAntColor_clicked();


   void on_radioButton_noStartingMaze_clicked();
   void on_radioButton_loadMazeFromFileOnStart_clicked();
   void on_radioButton_generateMazeOnStart_clicked();

private:
   void setStartingValue(const QString& optName, int val, QSpinBox* widget);
   void setStartingValue(const QString& optName, bool val, QCheckBox* widget);
   void setStartingValue(const QString& optName, const QColor& color);
   

   Ui::AppSettings* ui;
   QSettings settings{ "config.ini", QSettings::IniFormat };



   static inline const char* MAZE_WIDTH_TAG = "MazeWidth";
   static inline const char* MAZE_HEIGHT_TAG = "MazeHeight";
   static inline const char* TILE_SIZE_TAG = "TileSize";
   static inline const char* MARKER_SIZE_TAG = "MarkerSize";
   static inline const char* PATH_SIZE_TAG = "PathSize";
   static inline const char* ANIMATION_ENABLED_TAG = "AnimEnabled";
   static inline const char* ANIMATION_TIME_TAG = "AnimTime";
   static inline const char* VISUALIZE_TAG = "Visualize";
   static inline const char* STARTING_MAZE_TAG = "MazeOnStart";
   static inline const char* ANT_SIZE_TAG = "AntSize";
   static inline const char* ANT_COLOR_TAG = "AntsColor";
   static inline const char* ANT_ANIMATION_TIME_TAG = "AntsAnimationTime";
   static inline const char* ANT_CONST_NUMBER_ENABLED_TAG = "ConstNumberAntsEnabled";
   static inline const char* ANT_CONST_NUMBER_TAG = "ConstNumberAnts";

public:
   static inline const char* PATH_TO_LAST_MAZE_FILE_TAG = "LastMaze";
};

Q_DECLARE_METATYPE(AppSettings::StartingMaze);
