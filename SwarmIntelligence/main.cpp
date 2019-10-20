#include "MainWindow.h"

#include <QApplication>
#include <windows.h>
#include <optional>
#include <iostream>

int main(int argc, char* argv[])
{

   QApplication a(argc, argv);
   MainWindow w;
   if (argc > 1)
   {
      uint32_t tileSize = 10;
      uint32_t mazeHeight = 10;
      uint32_t mazeWidth = 10;
      uint32_t pathSize = 3;
      uint32_t markerSize = 20;

      int animFrameTime = 100;

      for (int i = 1; i < argc; ++i)
      {
         std::string arg = argv[i];

         if ("-tileSize" == arg || "--tS" == arg)
         {
            tileSize = std::stoul(argv[++i]);
         }
         else if ("-mazeHeight" == arg || "--mW" == arg)
         {
            mazeHeight = std::stoul(argv[++i]);
         }
         else if ("-mazeWidth" == arg || "--mH" == arg)
         {
            mazeWidth = std::stoul(argv[++i]);
         }
         else if ("-pathSize" == arg || "--pS" == arg)
         {
            pathSize = std::stoul(argv[++i]);
         }
         else if ("-markerSize" == arg || "--mS" == arg)
         {
            markerSize = std::stoul(argv[++i]);
         }
         else if ("-animFrameTime" == arg || "--aFT" == arg)
         {
            animFrameTime = std::stoul(argv[++i]);
         }
         else if("-help" == arg || "--h" == arg)
         {
            //add helper
         }
      }

      w.initializeMazeGen(mazeWidth, mazeHeight, tileSize, pathSize, markerSize);
      w.generateMaze(animFrameTime);
   }
   else
   {
      w.initializeMazeGen();
      w.generateMaze(100);
   }
   w.show();
   return a.exec();
}
