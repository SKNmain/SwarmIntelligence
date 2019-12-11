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
   //   this->ui->actionSave_maze_to_file->setEnabled(false);

   this->ui->splitter->setSizes({ 1000, 50 });

   connect(&Logger::getInstance(), &Logger::log, this->ui->logWidget, &LogWidget::logQStr);

   switch(this->settings.startingMazeStatus())
   {
   case AppSettings::LOAD_FROM_FILE:
   {
      loadMazeFromFile(this->settings.pathToMaze());
      this->ui->actionShow_shortest_path->setChecked(false);
      break;
   }
   case AppSettings::GENERATE_NEW:
   {
      this->on_actionGenerate_maze_triggered();
      break;
   }
   case AppSettings::NO_STARTING: break;
   }
}

MainWindow::~MainWindow()
{
   DELLPTR(this->mazeGenerator);
   DELLPTR(this->mazeSolver);
   DELLPTR(this->maze);

   DELLPTR(this->stepRenderingTimer);

   if(nullptr != this->antsTimer)
   {
      this->antsTimer->stop();
      DELLPTR(this->antsTimer);
   }

   DELLPTR(this->ui);
}

void MainWindow::generateMaze()
{
   emit Logger::getInstance().log("Maze generation...", LogWidget::LogLevel::INFO);
   QApplication::setOverrideCursor(Qt::WaitCursor);

   if(nullptr != this->antsTimer)
   {
      this->antsTimer->stop();
      DELLPTR(this->antsTimer);
   }

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
      this->maze = this->mazeGenerator->generateMazeStepByStep(this->settings.getMazeWidth(), this->settings.getMazeHeight());

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
            this->ui->graphicsView->clearMarkers();

            if(false == this->maze->isGeneratingFinished())
            {
               this->mazeGenerator->nextMazeGenerationStep(this->maze);
               this->ui->graphicsView->addMazeToScene(maze);
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
   this->maze = this->mazeGenerator->generateMaze(this->settings.getMazeWidth(), this->settings.getMazeHeight());
   //remove last visited cell markers
   //this->maze->removeMarkers();
   if(true == this->settings.isVisualize())
   {
      this->ui->graphicsView->addMazeToScene(maze);
   }
   this->ui->graphicsView->clearMarkers();
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
   this->ui->actionShow_shortest_path->setEnabled(enabled);

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
      this->ui->graphicsView->clearMarkers();
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
   if(nullptr != this->antsTimer)
   {
      this->antsTimer->stop();
      DELLPTR(this->antsTimer);
   }

   this->antsTimer = new QTimer;
   this->antsTimer->setInterval(this->settings.getAnimationTime());
   connect(&this->antsManager, &AntsManager::antsFinishedMaze, this->antsTimer, &QTimer::deleteLater);
   connect(this->antsTimer, &QTimer::timeout, this, [this]()
      {
         this->on_actionAnts_step_triggered();
      });
   this->antsTimer->start();
}

void MainWindow::on_actionShow_shortest_path_triggered()
{
   this->ui->graphicsView->setVisibleShortestWay(this->ui->actionShow_shortest_path->isChecked());
}

void MainWindow::on_actionSave_maze_to_file_triggered()
{
   if(nullptr != this->maze)
   {
      const QString& path = QFileDialog::getSaveFileName(this, "Select file with serialized maze", "", "Maze file(*.m);;All files (*.*)");
      if(false == path.isEmpty())
      {
         if(true == this->maze->serializeToFile(path.toStdString()))
         {
            //save last path
            this->settings.setLastMazePath(path);

            this->ui->graphicsView->setVisibleShortestWay(this->ui->actionShow_shortest_path->isChecked());

            emit Logger::getInstance().log("Successfully saved serialized maze to file " + path, LogWidget::LogLevel::INFO);
         }
         else
         {
            emit Logger::getInstance().log("Cannot open file to serialize maze", LogWidget::LogLevel::ERR);
         }
      }
   }
}

void MainWindow::loadMazeFromFile(const QString& path)
{
   if(false == path.isEmpty())
   {
      Maze* tempMaze = Maze::serializeFromFile(path.toStdString());
      if(nullptr != tempMaze)
      {
         //delete old one
         delete this->maze;

         this->maze = tempMaze;

         this->settings.setMazeHeight(this->maze->getHeight());
         this->settings.setMazeWidth(this->maze->getWidth());

         //clear view and display loaded maze
         this->ui->graphicsView->clear();
         if(true == this->settings.isVisualize())
         {
            this->ui->graphicsView->addMazeToScene(this->maze);
            this->ui->graphicsView->clearMarkers();
         }

         //save last path
         this->settings.setLastMazePath(path);

         emit Logger::getInstance().log("Successfully load serialized maze from file " + path, LogWidget::LogLevel::INFO);
      }
      else
      {
         emit Logger::getInstance().log("Cannot open file to deserialize maze", LogWidget::LogLevel::ERR);
      }
   }
   else
   {
      emit Logger::getInstance().log("Path with maze is empty!", LogWidget::LogLevel::ERR);
   }
}

void MainWindow::on_actionLoad_maze_to_file_triggered()
{
   const QString& path = QFileDialog::getOpenFileName(this, "Select file with serialized maze", "", "Maze file(*.m);;All files (*.*)");
   loadMazeFromFile(path);
}
