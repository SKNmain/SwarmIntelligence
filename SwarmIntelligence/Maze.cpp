#include "Maze.h"
#include <filesystem>
#include <fstream>
#include <QUuid>
#include "Logger.hpp"

Maze::Maze(uint32_t width, uint32_t height)
{
   this->width = width;
   this->height = height;

   this->uuid = QUuid::createUuid().toString();

   for(uint32_t y = 0; y < this->height; ++y)
   {
      std::vector<int> temp;
      for(uint32_t x = 0; x < this->width; ++x)
      {
         temp.push_back(0);
      }
      this->mazeArray.push_back(temp);
   }


   this->startingPoint.first = 0;
   this->startingPoint.second = 0;

   this->endPoint.first = width - 1;
   this->endPoint.second = height - 1;
}


std::pair<int, int> Maze::getStartingPoint() const
{
   return this->startingPoint;
}

std::pair<int, int> Maze::getEndPoint() const
{
   return this->endPoint;
}

void Maze::finishedGenerating()
{
   this->generatingFinished = true;
}

bool Maze::isGeneratingFinished() const
{
   return this->generatingFinished;
}

void Maze::setUuid(const QString& uuid)
{
   this->uuid = uuid;
}

QString Maze::getUuid() const
{
   return this->uuid;
}

bool Maze::serializeToFile(const std::string& fileName)
{
   bool rV = false;

   std::ofstream output(fileName, std::ios::out | std::ios::trunc);
   if(true == output.is_open())
   {
      output << this->getUuid().toStdString() << std::endl;
      output << this->startingPoint.first << " " << this->startingPoint.second << std::endl;
      output << this->endPoint.first << " " << this->endPoint.second << std::endl;
      output << this->width << " " << this->height << std::endl;

      output << "Maze:" << std::endl;

      for(const auto& row : this->mazeArray)
      {
         for(const auto& tile : row)
         {
            output << tile << " ";
         }
         output << std::endl;
      }
      output << std::endl;

      if(false == this->shortestWayArray.empty())
      {
         output << "ShortestWay: " << std::endl;
         for(const auto& row : this->shortestWayArray)
         {
            for(const auto& tile : row)
            {
               output << std::setw(2) << tile << " ";
            }
            output << std::endl;
         }
      }

      output.close();

      rV = true;
   }

   return rV;
}

Maze* Maze::serializeFromFile(const std::string& fileName)
{
   Maze* maze = nullptr;
   if(true == std::filesystem::exists(fileName))
   {
      std::ifstream inputFile(fileName);
      if(true == inputFile.is_open())
      {
         std::string temp;
         std::string uuid;
         int mazeWidth = 0;
         int mazeHeight = 0;
         std::pair<int, int> startingPoint{ 0, 0 };
         std::pair<int, int> endPoint{ 0, 0 };

         std::vector<std::vector<int>> mazeArray;
         std::vector<std::vector<int>> shortestWayArray;

         inputFile >> uuid;

         //get starting point
         inputFile >> startingPoint.first;
         inputFile >> startingPoint.second;

         //get end point
         inputFile >> endPoint.first;
         inputFile >> endPoint.second;

         //get size of maze
         inputFile >> mazeWidth;
         inputFile >> mazeHeight;

         inputFile >> temp;

         if(temp == "Maze:")
         {
            for(int y = 0; y < mazeHeight; ++y)
            {
               std::vector<int> row;
               for(int x = 0; x < mazeWidth; ++x)
               {
                  int cell;
                  inputFile >> cell;
                  row.push_back(cell);
               }
               mazeArray.push_back(row);
            }

            inputFile >> temp;
            if(temp == "ShortestWay:")
            {
               for(int y = 0; y < mazeHeight; ++y)
               {
                  std::vector<int> row;
                  for(int x = 0; x < mazeWidth; ++x)
                  {
                     int cell;
                     inputFile >> cell;
                     row.push_back(cell);
                  }
                  shortestWayArray.push_back(row);
               }
            }

            maze = new Maze(mazeWidth, mazeHeight);
            maze->setUuid(uuid.c_str());
            maze->mazeArray = mazeArray;
            maze->shortestWayArray = shortestWayArray;
            maze->finishedGenerating();
         }
      }
   }

   return maze;
}

uint32_t Maze::getHeight() const
{
   return this->height;
}

uint32_t Maze::getWidth() const
{
   return this->width;
}

