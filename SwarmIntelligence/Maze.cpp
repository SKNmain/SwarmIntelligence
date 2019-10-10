#include "Maze.h"

Maze::Maze(uint32_t width, uint32_t height, uint32_t tileWidth, uint32_t tileHeight)
{
   this->width = width;
   this->height = height;

   this->tileHeight = tileHeight;
   this->tileWidth = tileWidth;
}

void Maze::generateMaze()
{
   for (int i = 0; i < this->width; ++i)
   {
      std::vector<MazeTileType> row;
      for (int j = 0; j < this->width; ++j)
      {
         if(j % 2 == 0)
         {
            row.push_back(MazeTileType::Wall);
         }
         else
         {
            row.push_back(MazeTileType::Road);
         }
      }

      this->mazeArray.emplace_back(row);

   }
}

std::vector<std::vector<Maze::MazeTileType>> Maze::getMazeArray() const
{
   return this->mazeArray;
}

uint32_t Maze::getTileHeight() const
{
   return this->tileHeight;
}

uint32_t Maze::getTileWidth() const
{
   return this->tileWidth;
}
