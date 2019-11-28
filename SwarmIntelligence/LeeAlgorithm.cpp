#include "LeeAlgorithm.h"
#include <queue>
#include <QFile>
#include <QTextStream>


LeeAlgorithm::LeeAlgorithm()
{
}

void LeeAlgorithm::setVector(std::vector<std::vector<int>>& vector)
{
   for (uint32_t i = 0; i < this->maze->getHeight(); ++i)
   {
      std::vector<int> temp;
      for (uint32_t j = 0; j < this->maze->getWidth(); ++j)
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
   //if point is out of range it doesn't have to be considered at all
   if (nextX < 0 || nextY < 0 || nextX >= this->maze->getWidth() || nextY >= this->maze->getHeight())
   {
      return false;
   }
   //just to look pretier 
   const auto& tile = this->maze->mazeArray[curY][curX];

   //if the first coordinates are the same
   if (curX == nextX)
   {
      //0  
      //1
      //up value is lower so if we will substract current value an next and get positive value we want to go up
      if (curY - nextY > 0)
      {
         //now we check if there is a path
         if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
         {
            return true;
         }
      }
      else
      {
         //check if we can go down
         if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
         {
            return true;
         }
      }
   }
   else
   {  //now second coordinates are the same
      //same rule 0 1 2 - value more to the right is higher
      if (curX - nextX > 0)
      {
         //check
         if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
         {
            return true;
         }
      }
      else
      {
         //check
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
   //TODO - do wy have to destru something?
}

void LeeAlgorithm::solveMaze(Maze* maze)
{
   this->shortestWay.clear(); //vector with shortest path
   this->leeArray.clear(); //vector with all the distances from start - to analyze

   this->maze = maze; //given maze


   const auto& startingPoint = this->maze->getStartingPoint(); 
   const auto& endPoint = this->maze->getEndPoint();

   setVector(this->leeArray);
   setVector(this->shortestWay);

   //points that repesent all directions
   std::pair<int, int> direction[4];
   direction[0].first = 0; //up
   direction[0].second = -1;

   direction[1].first = 0; //down
   direction[1].second = 1;

   direction[2].first = -1; //left
   direction[2].second = 0;

   direction[3].first = 1; //right
   direction[3].second = 0;

   //vector of visited points
   std::vector<std::vector<bool>> visited; 

   //just setting vector
   for (int i = 0; i < maze->getHeight(); ++i)
   {
      visited.push_back(std::vector<bool>( this->maze->getWidth() ));
   }
   
   //we start the algorithm by adding starting point to the queue and setting vectors
   std::queue<Node> queueOfPoints;
   queueOfPoints.push({startingPoint.first, startingPoint.second, 0});
   visited[startingPoint.first][startingPoint.second] = true;
   leeArray[startingPoint.first][startingPoint.second] = 0;

   int xx, yy, dist;
   //algorithm: we add the point that is valid to the queue
   while (!queueOfPoints.empty())
   {
      Node temp = queueOfPoints.front();
      queueOfPoints.pop();

      //check for all 4 directions
      for (int i = 0; i < 4; ++i)
      {
         xx = temp.x + direction[i].first;
         yy = temp.y + direction[i].second;
         dist = temp.dist;
         if (isValid(temp.x, temp.y, xx, yy) && !visited[yy][xx])
         {
            visited[yy][xx] = true;
            queueOfPoints.push({ xx, yy, dist + 1});
            leeArray[yy][xx] = dist + 1;
         }
      }
   }
   //shortest path - we go from the end and look for the value lower by 1 from
   //value that I am standing right now
   //when we get to the start, we are finished
   std::pair<int, int> currPoint = endPoint;

   this->shortestWay[endPoint.second][endPoint.first] = Maze::CELL_SHORTEST;
   std::pair<int, int> lastPoint;
   std::pair<int, int> minPoint;
   std::pair<int, int> temp;
   bool first = true;
   bool notFinished = true;
   int curDist = leeArray[endPoint.second][endPoint.first];

   while (notFinished)
   {
      for (int i = 0; i < 4; ++i)
      {
         temp.first = currPoint.first + direction[i].first;
         temp.second = currPoint.second + direction[i].second;
         if (temp.first < 0 || temp.second < 0 || temp.first >= this->maze->getWidth() || temp.second >= this->maze->getHeight())
         {
            continue;
         }

         if (leeArray[temp.second][temp.first] == (curDist - 1))
         {
            minPoint.first = temp.first;
            minPoint.second = temp.second;
            curDist -= 1;
            break;
         }
      }

      lastPoint.first = currPoint.first;
      lastPoint.second = currPoint.second;

      currPoint.first = minPoint.first;
      currPoint.second = minPoint.second;
      
      this->shortestWay[currPoint.second][currPoint.first] = Maze::CELL_SHORTEST;

      if (currPoint.first == startingPoint.first && currPoint.second == startingPoint.second)
      {
         notFinished = false;
      }
   }
   
   maze->shortestWayArray = this->shortestWay;
}

std::vector<std::vector<int>> LeeAlgorithm::getLeesArray() const
{
   return this->leeArray;
}
