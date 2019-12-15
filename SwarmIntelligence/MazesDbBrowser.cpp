#include "MazesDbBrowser.h"
#include "ui_MazesDbBrowser.h"
#include "DbManager.h"
#include "Maze.h"


#include <QDebug>

MazesDbBrowser::MazesDbBrowser(AppSettings* settings, QWidget* parent) :
   QDialog(parent),
   ui(new Ui::MazesDbBrowser)
{
   this->ui->setupUi(this);

   setWindowFlag(Qt::WindowContextHelpButtonHint, false);

   this->model = new QSqlTableModel(this, DbManager::getDB());
   this->model->setTable("Mazes");
   this->model->select();


   this->idIdx = this->model->fieldIndex("uuid");
   this->heightIdx = this->model->fieldIndex("height");
   this->widthIdx = this->model->fieldIndex("width");
   this->descIdx = this->model->fieldIndex("description");
   this->mazeArrayIdx = this->model->fieldIndex("array");
   this->shortestWayArrayIdx = this->model->fieldIndex("shortestWayArray");


   this->model->setHeaderData(this->idIdx, Qt::Horizontal, "UUID");
   this->model->setHeaderData(this->heightIdx, Qt::Horizontal, "Height");
   this->model->setHeaderData(this->widthIdx, Qt::Horizontal, "Width");
   this->model->setHeaderData(this->descIdx, Qt::Horizontal, "Description");


   this->ui->mazesTableView->setModel(this->model);

   this->ui->mazesTableView->setColumnHidden(this->mazeArrayIdx, true);
   this->ui->mazesTableView->setColumnHidden(this->shortestWayArrayIdx, true);
   this->ui->mazesTableView->horizontalHeader()->setSectionResizeMode(this->descIdx, QHeaderView::ResizeMode::Stretch);


   this->ui->graphicsView->initSettings(settings);
   this->ui->graphicsView->hide();

}

MazesDbBrowser::~MazesDbBrowser()
{
   delete ui;
}

void MazesDbBrowser::refresh()
{
   this->model->select();
}

void MazesDbBrowser::on_mazesTableView_doubleClicked(const QModelIndex& index)
{
   Q_UNUSED(index);
   on_pushButtonmazePreview_clicked();
}

void MazesDbBrowser::on_pushButton_hide_clicked()
{
   this->ui->graphicsView->hide();
}

void MazesDbBrowser::on_pushButtonmazePreview_clicked()
{
   Maze* tempMaze = getMazeFromSelectedDBRow();
   if(nullptr != tempMaze)
   {
      this->ui->graphicsView->addMazeToScene(tempMaze);
      this->ui->graphicsView->show();
      this->ui->graphicsView->clearMarkers();
      delete tempMaze;
   }
}

void MazesDbBrowser::on_pushButton_removeMaze_clicked()
{
   const auto& selectedRows = this->ui->mazesTableView->selectionModel()->selectedRows();
   if(false == selectedRows.empty())
   {
      this->ui->mazesTableView->model()->removeRow(selectedRows[0].row());
      this->model->submitAll();
      this->ui->mazesTableView->reset();
      this->model->select();
   }
}

void MazesDbBrowser::on_pushButton_loadMaze_clicked()
{
   Maze* tempMaze = getMazeFromSelectedDBRow();
   if(nullptr != tempMaze)
   {
      emit this->loadMazeFromDB(tempMaze);
   }
}

Maze* MazesDbBrowser::getMazeFromSelectedDBRow() const
{
   Maze* tempMaze = nullptr;
   const auto& selectedRows = this->ui->mazesTableView->selectionModel()->selectedRows();
   if(false == selectedRows.empty())
   {
      int row = selectedRows[0].row();

      int mazeWidth = this->model->data(this->model->index(row, this->widthIdx)).toInt();
      int mazeHeight = this->model->data(this->model->index(row, this->heightIdx)).toInt();

      std::pair<int, int> startingPoint{ 0, 0 };
      std::pair<int, int> endPoint{ 0, 0 };

      tempMaze = new Maze(mazeWidth, mazeHeight);
      tempMaze->mazeArray.clear();

      const auto& cells = this->model->data(this->model->index(row, this->mazeArrayIdx)).toString().split(" ", QString::SplitBehavior::SkipEmptyParts);
      for(int y = 0; y < mazeHeight; ++y)
      {
         std::vector<int> row;
         for(int x = 0; x < mazeWidth; ++x)
         {
            row.push_back(cells[y * mazeWidth + x].toInt());
         }
         tempMaze->mazeArray.push_back(row);
      }
      const auto& shortestWayCells = this->model->data(this->model->index(row, this->shortestWayArrayIdx)).toString().split(" ", QString::SplitBehavior::SkipEmptyParts);
      if(false == shortestWayCells.empty())
      {
         for(int y = 0; y < mazeHeight; ++y)
         {
            std::vector<int> row;
            for(int x = 0; x < mazeWidth; ++x)
            {
               row.push_back(shortestWayCells[y * mazeWidth + x].toInt());
            }
            tempMaze->shortestWayArray.push_back(row);
         }
      }

      tempMaze->finishedGenerating();
   }
   return tempMaze;
}
