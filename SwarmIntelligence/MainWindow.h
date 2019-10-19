#pragma once

#include <QMainWindow>

namespace Ui { class MainWindow; }

class Maze;

class MainWindow : public QMainWindow
{
   Q_OBJECT;

public:
   MainWindow(QWidget* parent = nullptr);
   ~MainWindow();
   bool showStep();

private:
   Ui::MainWindow* ui;
   QTimer* stepRenderingTimer;
   Maze* maze;
};
