#include "MainWindow.h"
#include "ui_MainWindowUI.h"
#include "Maze.h"
#include <QShortcut>
#include <QTimer>


MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   this->maze = new Maze(15, 30, 10, 10);

   this->stepRenderingTimer = new QTimer();
   this->stepRenderingTimer->setInterval(100);
   connect(stepRenderingTimer, &QTimer::timeout, this, [this]()
      {
         showStep();
      });

   this->stepRenderingTimer->start();
}

bool MainWindow::showStep()
{
   if (false == maze->isMazeDone)
   {
      this->maze->generateMaze();
      this->ui->graphicsView->addMazeToScene(*maze);
      return true;
   }
   return false;
}

MainWindow::~MainWindow()
{
   delete ui;
}

