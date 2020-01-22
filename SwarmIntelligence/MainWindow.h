#pragma once

#include <QMainWindow>
#include "AppSettings.h"
#include "MazeGenerator_I.h"
#include "MazeSolver_I.h"
#include "AntsManager.h"

namespace Ui { class MainWindow; }

#define NO_ANIMATION 0

class Maze;

class MainWindow : public QMainWindow
{
   Q_OBJECT;

public:
   MainWindow(QWidget* parent = nullptr);
   ~MainWindow();

   void generateMaze();

private slots:
   void on_actionGenerate_maze_triggered();
   void on_actionSettings_triggered();

   void on_actionAbout_triggered();
   void on_actionExit_triggered();
   void on_actionSave_maze_img_to_file_triggered();

   void on_actionStop_generating_triggered();
   void on_actionGenerate_shortest_path_triggered();

   void on_actionAnts_step_triggered();

   void on_actionSwarm_intelligence_triggered();


   void on_actionRun_ants_triggered();

   void on_actionShow_shortest_path_triggered();

   void on_actionSave_maze_to_file_triggered();
   void loadMazeFromFile(const QString& path);

   void on_actionLoad_maze_to_file_triggered();

   void on_actionRun_quick_ants_triggered();

private:
   void setActionEnabled(bool enabled);
   void generateWholeMaze();


   Maze* maze = nullptr;
   MazeGenerator_I* mazeGenerator = nullptr;
   MazeSolver_I* mazeSolver = nullptr;

   //temporary var
   int antSolverStepsCounter = 0;

   QTimer* antsTimer = nullptr;
   QTimer* stepRenderingTimer = nullptr;
   AppSettings settings;
   AntsManager antsManager{ &settings };
   Ui::MainWindow* ui;
};

