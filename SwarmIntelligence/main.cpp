#include "MainWindow.h"

#include <QApplication>
#include <windows.h>
#include <optional>
#include <iostream>

int main(int argc, char* argv[])
{

   QApplication a(argc, argv);
   MainWindow w;
   w.show();
   return a.exec();
}
