#include "MainWindow.h"

#include <QApplication>
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    bool con = true;
   // while (con)
    //{
      con = w.showStep();
      w.show();
      //Sleep(1000);
    //}
    
    return a.exec();
}
