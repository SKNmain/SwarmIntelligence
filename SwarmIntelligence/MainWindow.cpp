#include "MainWindow.h"
#include "ui_MainWindowUI.h"
#include "Maze.h"
#include <QShortcut>
#include <QTimer>
#include <QFileDialog>
#include "AboutDialog.h"
#include "Logger.hpp"


MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent), ui(new Ui::MainWindow)
{
   this->ui->setupUi(this);
   this->ui->actionStop_generating->setEnabled(false);

   this->ui->splitter->setSizes({ 1000, 50 });

   connect(&Logger::getInstance(), &Logger::log, this->ui->logWidget, &LogWidget::logQStr);
}

void MainWindow::generateWholeMaze()
{
   this->maze->generateMaze();
   //remove last visited cell markers
   this->maze->removeMarkers();
   if(true == this->settings.isVisualize())
   {
      this->ui->graphicsView->addMazeToScene(*maze);
   }
}
MainWindow::~MainWindow()
{
   if(nullptr != this->maze)
   {
      delete this->maze;
   }

   delete ui;
}

void MainWindow::initializeMazeGen(uint32_t mazeWidth, uint32_t mazeHeight, uint32_t tileSize, uint32_t pathWidth, uint32_t markerSize)
{
   this->ui->graphicsView->clear();
   if(nullptr != this->maze)
   {
      delete this->maze;
   }

   this->maze = new Maze(mazeWidth, mazeHeight, tileSize, pathWidth, markerSize);
}

void MainWindow::generateMaze(int animationFrameTime)
{
   emit Logger::getInstance().log("Maze generation...", LogWidget::LogLevel::INFO);
   QApplication::setOverrideCursor(Qt::WaitCursor);

   //generate whole maze
   if(0 == animationFrameTime)
   {
      generateWholeMaze();
      emit Logger::getInstance().log("Finished maze generation.", LogWidget::LogLevel::INFO);
      QApplication::restoreOverrideCursor();
   }
   else//generate maze step by step with animationFrameTime as interval between steps
   {
      //disable actions in menu
      setActionEnabled(false);

      this->stepRenderingTimer = new QTimer();
      this->stepRenderingTimer->setInterval(animationFrameTime);
      connect(stepRenderingTimer, &QTimer::timeout, this, [this]()
         {
            generateMazeStep();
            if(true == this->maze->isMazeGenerationFinished())
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

void MainWindow::generateMazeStep()
{
   if(false == maze->isMazeGenerationFinished())
   {
      this->maze->removeMarkers();
      this->maze->generateStep();
      this->ui->graphicsView->addMazeToScene(*maze);
   }
}

void MainWindow::on_actionExit_triggered()
{
   QApplication::exit();
}

void MainWindow::setActionEnabled(bool enabled)
{
   this->ui->actionGenerate_maze->setEnabled(enabled);
   this->ui->actionStop_generating->setEnabled(!enabled);
}

void MainWindow::on_actionGenerate_maze_triggered()
{
   //initialize maze 
   initializeMazeGen(this->settings.getMazeWidth(), this->settings.getMazeHeight(), this->settings.getTileSize(), this->settings.getPathSize(), this->settings.getMarkerSize());

   //decide, if user want to get whole maze immediately or want to get animation
   if(false == this->settings.isAnimationEnabled())
   {
      this->generateMaze();
   }
   else
   {
      this->generateMaze(this->settings.getAnimationTime());
   }
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

void MainWindow::on_actionStop_generating_triggered()
{
   if(nullptr != this->maze)
   {
      if(false == this->maze->isMazeGenerationFinished())
      {
         this->stepRenderingTimer->stop();
         emit Logger::getInstance().log("Maze generation interrupted.", LogWidget::LogLevel::ERR);

         QApplication::restoreOverrideCursor();
         //enable actions in menu
         setActionEnabled(true);
      }
   }
}

void MainWindow::on_actionAbout_triggered()
{
   AboutDialog().exec();
}
