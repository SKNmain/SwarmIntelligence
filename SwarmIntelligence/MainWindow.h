#pragma once

#include <QMainWindow>

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

private:
   void generateMazeStep();
   void generateWholeMaze();

   Ui::MainWindow* ui;
   QTimer* stepRenderingTimer = nullptr;
   Maze* maze = nullptr;
};

