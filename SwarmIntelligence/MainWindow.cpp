#include "MainWindow.h"
#include "ui_MainWindowUI.h"
#include "Maze.h"
#include <windows.h>


MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
}

bool MainWindow::showStep()
{
   Maze maze(10, 10, 10, 10);
  // if (!maze.isMazeDone)
   //{
      maze.generateMaze();
      this->ui->graphicsView->addMazeToScene(maze);
      return true;
   //}
  // return false;
}

MainWindow::~MainWindow()
{
   delete ui;
}

