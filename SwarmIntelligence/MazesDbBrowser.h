#pragma once

#include <QDialog>
#include <QSqlRelationalTableModel>

namespace Ui {
   class MazesDbBrowser;
}

class AppSettings;
class Maze;

class MazesDbBrowser : public QDialog
{
   Q_OBJECT;

public:
   explicit MazesDbBrowser(AppSettings* settings, QWidget* parent = nullptr);
   ~MazesDbBrowser();

   void refresh();

signals:
   void loadMazeFromDB(Maze* maze);

private slots:
    void on_mazesTableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_hide_clicked();

    void on_pushButtonmazePreview_clicked();

    void on_pushButton_removeMaze_clicked();

    void on_pushButton_loadMaze_clicked();

private:
   Maze* getMazeFromSelectedDBRow() const;

   Ui::MazesDbBrowser* ui;
   QSqlTableModel* model;
   AppSettings* settings;
   int idIdx;
   int heightIdx;
   int widthIdx;
   int descIdx;
   int mazeArrayIdx;
   int shortestWayArrayIdx;
};

