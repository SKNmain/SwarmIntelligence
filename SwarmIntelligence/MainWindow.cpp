#include "MainWindow.h"
#include "ui_MainWindowUI.h"
#include "Maze.h"

MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   Maze maze(50, 50);
   maze.generateMaze();
   this->ui->graphicsView->addMazeToScene(maze);
}

MainWindow::~MainWindow()
{
   delete ui;
}

