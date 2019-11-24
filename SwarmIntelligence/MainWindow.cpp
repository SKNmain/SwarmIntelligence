#include "MainWindow.h"
#include "ui_MainWindowUI.h"
#include "Maze.h"
#include <QShortcut>
#include <QTimer>
#include <QFileDialog>
#include <QThread>
#include "AboutDialog.h"
#include "Logger.hpp"
#include "StackMazeGenerator.h"
#include "Helper.h"


MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent), ui(new Ui::MainWindow)
{
   this->ui->setupUi(this);
   this->ui->actionStop_generating->setEnabled(false);

   this->ui->splitter->setSizes({ 1000, 50 });

   connect(&Logger::getInstance(), &Logger::log, this->ui->logWidget, &LogWidget::logQStr);

}

MainWindow::~MainWindow()
{
   DELLPTR(this->mazeGenerator);
   DELLPTR(this->maze);
   DELLPTR(this->stepRenderingTimer);
   DELLPTR(this->ui);
}

void MainWindow::generateMaze()
{
   emit Logger::getInstance().log("Maze generation...", LogWidget::LogLevel::INFO);
   QApplication::setOverrideCursor(Qt::WaitCursor);

   //generate whole maze
   if(false == this->settings.isAnimationEnabled())
   {
      generateWholeMaze();

      emit Logger::getInstance().log("Finished maze generation.", LogWidget::LogLevel::INFO);
      QApplication::restoreOverrideCursor();
   }
   else//generate maze step by step with animationFrameTime as interval between steps
   {
      //disable actions in menu
      setActionEnabled(false);

      //get pointer to new maze
      this->maze = this->mazeGenerator->generateMazeStepByStep(this->settings.getMazeWidth(), this->settings.getMazeHeight(), this->settings.getTileSize(), this->settings.getPathSize());

      //stop timer for sure
      if(nullptr != this->stepRenderingTimer) 
      {
         this->stepRenderingTimer->stop();
      }

      //delete timer
      DELLPTR(this->stepRenderingTimer);

      //create timer
      this->stepRenderingTimer = new QTimer();
      this->stepRenderingTimer->setInterval(this->settings.getAnimationTime());
      connect(stepRenderingTimer, &QTimer::timeout, this, [this]()
         {
            if(false == this->maze->isGeneratingFinished())
            {
               this->maze->removeMarkers();
               this->mazeGenerator->nextMazeGenerationStep(this->maze);
               this->ui->graphicsView->addMazeToScene(*maze);
            }
            else
            {
               this->stepRenderingTimer->stop();
               emit Logger::getInstance().log("Finished maze generation.", LogWidget::LogLevel::INFO);

               QApplication::restoreOverrideCursor();
               //enable actions in menu
               setActionEnabled(true);
               
            }
         });
      this->stepRenderingTimer->start();
   }
}

void MainWindow::generateWholeMaze()
{
   this->maze = this->mazeGenerator->generateMaze(this->settings.getMazeWidth(), this->settings.getMazeHeight(), this->settings.getTileSize(), this->settings.getPathSize());
   //remove last visited cell markers
   //this->maze->removeMarkers();
   if(true == this->settings.isVisualize())
   {
      this->ui->graphicsView->addMazeToScene(*maze);
   }
}

void MainWindow::on_actionGenerate_maze_triggered()
{
   //create maze generator
   this->mazeGenerator = new StackMazeGenerator;

   //clear old maze view
   this->ui->graphicsView->clear();

   //initialize maze
   DELLPTR(this->maze);


   //decide, if user want to get whole maze immediately or want to get animation
   this->generateMaze();
}

void MainWindow::on_actionStop_generating_triggered()
{
   if(nullptr != this->maze)
   {
      if(true == this->stepRenderingTimer->isActive())
      {
         this->stepRenderingTimer->stop();
         emit Logger::getInstance().log("Maze generation interrupted.", LogWidget::LogLevel::ERR);

         QApplication::restoreOverrideCursor();
         //enable actions in menu
         setActionEnabled(true);
      }
   }
}

void MainWindow::setActionEnabled(bool enabled)
{
   this->ui->actionGenerate_maze->setEnabled(enabled);
   this->ui->actionStop_generating->setEnabled(!enabled);
}

void MainWindow::on_actionGenerate_shortest_path_triggered()
{
   this->ui->graphicsView->createShortestPath(*this->maze);
}

void MainWindow::on_actionSettings_triggered()
{
   this->settings.exec();
}

void MainWindow::on_actionSave_maze_img_to_file_triggered()
{
   const QString imagePath = QFileDialog::getSaveFileName(this, "Image save");
   if(false == imagePath.isEmpty() && imagePath != ".")
   {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      if(true == this->ui->graphicsView->saveScreenshot(imagePath))
      {
         emit Logger::getInstance().log("Maze screenshot was successful saved. [" + imagePath + "].", LogWidget::LogLevel::INFO);
      }
      else
      {
         emit Logger::getInstance().log("Cannot save maze screenshot. [" + imagePath + "].", LogWidget::LogLevel::ERR);
      }
      QApplication::restoreOverrideCursor();
   }
}

void MainWindow::on_actionAbout_triggered()
{
   AboutDialog().exec();
}

void MainWindow::on_actionExit_triggered()
{
   QApplication::exit();
}
