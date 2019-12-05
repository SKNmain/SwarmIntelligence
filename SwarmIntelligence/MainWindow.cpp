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
#include "LeeAlgorithm.h"


MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent), ui(new Ui::MainWindow)
{
   this->ui->setupUi(this);

   this->ui->graphicsView->initSettings(&this->settings);

   this->ui->actionStop_generating->setEnabled(false);
   this->ui->actionGenerate_shortest_path->setEnabled(false);
   this->ui->actionAnts_step->setEnabled(false);
   this->ui->actionRun_ants->setEnabled(false);

   this->ui->splitter->setSizes({ 1000, 50 });

   connect(&Logger::getInstance(), &Logger::log, this->ui->logWidget, &LogWidget::logQStr);

   if(true == this->settings.isGenerateMazeOnStart())
   {
      this->on_actionGenerate_maze_triggered();
   }
}

MainWindow::~MainWindow()
{
   DELLPTR(this->mazeGenerator);
   DELLPTR(this->mazeSolver);
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
      setActionEnabled(true);
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
            this->ui->graphicsView->clearAntsMarkersFromScene();

            if(false == this->maze->isGeneratingFinished())
            {
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
   this->ui->graphicsView->clearAntsMarkersFromScene();
}

void MainWindow::on_actionGenerate_maze_triggered()
{
   //create maze generator
   this->mazeGenerator = new StackMazeGenerator;

   //clear old maze view
   this->ui->graphicsView->clear();

   //initialize maze
   DELLPTR(this->maze);

   this->ui->actionAnts_step->setEnabled(false);
   this->ui->actionRun_ants->setEnabled(false);

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
   this->ui->actionGenerate_shortest_path->setEnabled(enabled);

   this->ui->actionStop_generating->setEnabled(!enabled);
}

void MainWindow::on_actionGenerate_shortest_path_triggered()
{
   if(nullptr != this->maze)
   {
      emit Logger::getInstance().log("Searching for shortest path in maze ...", LogWidget::LogLevel::INFO);

      this->ui->graphicsView->clearShortestWayTiles();

      DELLPTR(this->mazeSolver);
      this->mazeSolver = new LeeAlgorithm;
      this->mazeSolver->solveMaze(this->maze);


      this->ui->graphicsView->drawShortestPath(this->maze);
      this->ui->graphicsView->setVisibleShortestWay(this->ui->actionShow_shortest_path->isChecked());

      emit Logger::getInstance().log("Finished", LogWidget::LogLevel::INFO);
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

void MainWindow::on_actionAbout_triggered()
{
   AboutDialog().exec();
}

void MainWindow::on_actionExit_triggered()
{
   QApplication::exit();
}

void MainWindow::on_actionAnts_step_triggered()
{
   if(nullptr != this->maze && true == this->maze->isGeneratingFinished())
   {
      this->antsManager.step();
      this->antSolverStepsCounter++;
      if(true == this->settings.isVisualize())
      {
         this->ui->graphicsView->renderAnts(this->antsManager);
      }
   }
   else
   {
      emit Logger::getInstance().log("You cannot do that, bro!", LogWidget::LogLevel::ERR);
   }
}

void MainWindow::on_actionSwarm_intelligence_triggered()
{
   if(nullptr != this->maze && true == this->maze->isGeneratingFinished())
   {
      this->ui->graphicsView->clearAntsMarkersFromScene();
      this->antsManager.initialize(this->maze);
      this->antSolverStepsCounter = 0;

      connect(&this->antsManager, &AntsManager::antsFinishedMaze, this, [this]()
      {
            emit Logger::getInstance().log(tr("Ant finished maze\nSteps: %1").arg(QString::number(this->antSolverStepsCounter)), 
               LogWidget::LogLevel::INFO);
      });

      this->ui->actionAnts_step->setEnabled(true);
      this->ui->actionRun_ants->setEnabled(true);
   }
   else
   {
      emit Logger::getInstance().log("You cannot do that, bro!", LogWidget::LogLevel::ERR);
   }
}

void MainWindow::on_actionRun_ants_triggered()
{
   QTimer* timer = new QTimer;
   timer->setInterval(this->settings.getAnimationTime());
   connect(&this->antsManager, &AntsManager::antsFinishedMaze, timer, &QTimer::deleteLater);
   connect(timer, &QTimer::timeout, this, [this]()
      {
         this->on_actionAnts_step_triggered();
      });
   timer->start();
}

void MainWindow::on_actionShow_shortest_path_triggered()
{
   this->ui->graphicsView->setVisibleShortestWay(this->ui->actionShow_shortest_path->isChecked());
}

void MainWindow::on_actionSave_maze_to_file_triggered()
{

}

void MainWindow::on_actionLoad_maze_to_file_triggered()
{

}
