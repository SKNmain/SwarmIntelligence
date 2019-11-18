#pragma once

#include <QMainWindow>
#include "AppSettings.h"

namespace Ui { class MainWindow; }

class Maze;

class MainWindow : public QMainWindow
{
   Q_OBJECT;

public:
   MainWindow(QWidget* parent = nullptr);
   ~MainWindow();

   void initializeMazeGen(uint32_t mazeWidth = 10, uint32_t mazeHeight = 10, uint32_t tileSize = 10,
                          uint32_t pathWidth = 3, uint32_t markerSize = 20);
   void generateMaze(int animationFrameTime = 0);

private slots:
   void on_actionGenerate_maze_triggered();
   void on_actionSettings_triggered();

   void on_actionExit_triggered();
   void on_actionSave_maze_img_to_file_triggered();

   void on_actionStop_generating_triggered();

   void on_actionAbout_triggered();

   void on_actionGenerate_shortest_path_triggered();

private:
   void setActionEnabled(bool enabled);
   void generateMazeStep();
   void generateWholeMaze();

   Ui::MainWindow* ui;
   QTimer* stepRenderingTimer = nullptr;
   Maze* maze = nullptr;

   AppSettings settings;
};

