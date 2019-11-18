#include "LeeAlgorithm.h"
#include <queue>
#include <QFile>
#include <QTextStream>


LeeAlgorithm::LeeAlgorithm(const Maze &givenMaze)
{
   this->givenArray = givenMaze.getMazeArray();
   this->height = givenMaze.getHeight();
   this->width = givenMaze.getWidth();

   for (uint32_t y = 0; y < this->height; ++y)
   {
      std::vector<int> temp;
      for (uint32_t x = 0; x < this->width; ++x)
      {
         temp.push_back(0);
      }
      this->leeArray.push_back(temp);
      this->showArray.push_back(temp);
   }

   this->endPoint = givenMaze.getEndPoint();
   this->startingPoint = givenMaze.getStartingPoint();
   //Maze::CellIdentifier::
}

void LeeAlgorithm::setVector(std::vector<std::vector<int>> vector)
{
   for (int i = 0; i < this->height; ++i)
   {
      std::vector<int> temp;
      for (int j = 0; j < this->width; ++j)
      {
         temp.push_back(0);
      }
      vector.push_back(temp);
   }
}

struct Node
{
   int x, y, dist;
};

bool LeeAlgorithm::isValid(int curX, int curY, int nextX, int nextY)
{
   if (nextX < 0 || nextY < 0 || nextX >= this->width  || nextY >= this->height)
   {
      return false;
   }
   auto tile = this->givenArray[curY][curX];

   bool s = false;
   bool n = false;
   bool w = false;
   bool e = false;
   if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
   {
      n = true;
   }
   if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
   {
      s = true;
   }
   if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
   {
      e = true;
   }
   if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
   {
      w = true;
   }

   if (curX == nextX)
   {
      if (curY - nextY > 0)
      {
         if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
         {
            return true;
         }
      }
      else
      {
         if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
         {
            return true;
         }
      }
   }
   else
   {
      if (curX - nextX > 0)
      {
         if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
         {
            return true;
         }
      }
      else
      {
         if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
         {
            return true;
         }
      }
   }
   return false;
}


LeeAlgorithm::~LeeAlgorithm()
{
}

void LeeAlgorithm::solveMaze()
{

   setVector(this->leeArray);
   setVector(this->showArray);
   std::pair<int, int> direction[4];
   direction[0].first = 0; //up
   direction[0].second = -1;

   direction[1].first = 0; //down
   direction[1].second = 1;

   direction[2].first = -1; //left
   direction[2].second = 0;

   direction[3].first = 1; //right
   direction[3].second = 0;

   std::vector<std::vector<bool>> visited;
   
   for (int i = 0; i < height; ++i)
   {
      visited.push_back(std::vector<bool>( this->width ));
   }

  // memset(visited, 0, sizeof(visited[0][0]) * this->width * this->height);
   std::queue<Node> queueOfPoints;
   queueOfPoints.push({startingPoint.first, 
      startingPoint.second, 0});
   visited[startingPoint.first][startingPoint.second] = true;
   leeArray[startingPoint.first][startingPoint.second] = 0;

   int xx, yy, dist;
   while (!queueOfPoints.empty())
   {
      Node temp = queueOfPoints.front();
      queueOfPoints.pop();

      for (int i = 0; i < 4; ++i)
      {
         xx = temp.x + direction[i].first;
         yy = temp.y + direction[i].second;
         dist = temp.dist;
         if (isValid(temp.x, temp.y, xx, yy) && !visited[xx][yy])
         {
            visited[yy][xx] = true;
            queueOfPoints.push({ xx, yy, dist + 1});
            leeArray[yy][xx] = dist + 1;
            //break;
         }

      }
   }

   std::pair<int, int> currPoint = this->startingPoint;

   this->showArray[this->startingPoint.first][this->startingPoint.second] = Maze::CELL_SHORTEST;
   std::pair<int, int> lastPoint;
   std::pair<int, int> minPoint;
   std::pair<int, int> temp;
   bool first = true;
   bool notFinished = true;
   
   while (notFinished)
   {  bool firstLoop = true;
      for (int i = 0; i < 4; ++i)
      {
         temp.first = currPoint.first + direction[i].first;
         temp.second = currPoint.second + direction[i].second;
         if (temp.first < 0 || temp.second < 0)
         {
            continue;
         }

         if (firstLoop) 
         {
            firstLoop = false;
            minPoint.first = temp.first;
            minPoint.second = temp.second;
         }
         else if (first || (temp.first != lastPoint.first || temp.second != lastPoint.second))
         {
            first = false;
            if (leeArray[minPoint.second][minPoint.first] > leeArray[temp.second][temp.first])
            {
               minPoint.first = temp.first;
               minPoint.second = temp.second;
            }
         }
         
      }
      lastPoint.first = currPoint.first;
      lastPoint.second = currPoint.second;

      currPoint.first = minPoint.first;
      currPoint.second = minPoint.second;

      if (currPoint.first == endPoint.first && currPoint.second == endPoint.second)
      {
         notFinished = false;
      }

      this->showArray[currPoint.first][currPoint.second] = Maze::CELL_SHORTEST;

   }
}

std::vector<std::vector<int>> LeeAlgorithm::getLeesArray() const
{
   return this->leeArray;
}

std::vector<std::vector<int>> LeeAlgorithm::getArrayToDraw() const
{
   return this->showArray;
}
