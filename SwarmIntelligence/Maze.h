#pragma once
#include <QColor>
#include <vector>

class Maze
{
public:
   Maze(uint32_t width, uint32_t height, uint32_t tileWidth = 20, uint32_t tileHeight = 20);

   enum class MazeTileType { Wall, Road, Start, End };

   void generateMaze();

   std::vector<std::vector<MazeTileType>> getMazeArray() const;
   uint32_t getTileHeight() const;
   uint32_t getTileWidth() const;
private:
   uint32_t width;
   uint32_t height;
   uint32_t tileWidth;
   uint32_t tileHeight;


   std::vector<std::vector<MazeTileType>> mazeArray;
};

