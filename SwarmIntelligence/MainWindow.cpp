#include "MainWindow.h"
#include "ui_MainWindowUI.h"
#include "Maze.h"
#include <QShortcut>
#include <QTimer>


MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent), ui(new Ui::MainWindow)
{
   this->ui->setupUi(this);

}

void MainWindow::generateMazeStep()
{
   if (false == maze->isMazeDone)
   {
      this->maze->removeMarkers();
      this->maze->generateStep();
      this->ui->graphicsView->addMazeToScene(*maze);  
   }
}

void MainWindow::generateWholeMaze()
{
   this->maze->removeMarkers();
   this->maze->generateMaze();
   this->ui->graphicsView->addMazeToScene(*maze);
}
MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::initializeMazeGen(uint32_t mazeWidth, uint32_t mazeHeight, uint32_t tileSize, uint32_t pathWidth, uint32_t markerSize)
{
   this->maze = new Maze(mazeWidth, mazeHeight, tileSize, pathWidth, markerSize);
}

void MainWindow::generateMaze(int animationFrameTime)
{
   if(0 == animationFrameTime)
   {
      generateWholeMaze();
   }
   else
   {
      this->stepRenderingTimer = new QTimer();
      this->stepRenderingTimer->setInterval(100);
      connect(stepRenderingTimer, &QTimer::timeout, this, [this]()
         {
            generateMazeStep();
            if (this->maze->isMazeDone == true)
            {
               this->stepRenderingTimer->stop();
            }
         });
      this->stepRenderingTimer->start();
   }

}

