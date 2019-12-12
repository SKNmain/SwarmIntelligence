#include "Ant.h"
#include "Maze.h"

RandGen<std::knuth_b>Ant::randGen = RandGen<std::knuth_b>(time(NULL));

Ant::Ant(int _id, int _x, int _y)
   : id(_id), pos(_x, _y), lastPos(_x, _y)
{
}

std::optional<Marker> Ant::update(int tile, const std::vector<Marker>& surrMarkers)
{
   std::optional<Marker> rVMarker = std::nullopt;

   //if(surrMarkers.size())
   //{
   //}

   if(false == this->endFoundChangeToBlue)
   {

      std::vector<int> road;
      if(Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
      {
         road.push_back(Maze::CELL_PATH_N);
      }
      if(Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
      {
         road.push_back(Maze::CELL_PATH_S);
      }
      if(Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
      {
         road.push_back(Maze::CELL_PATH_E);
      }
      if(Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
      {
         road.push_back(Maze::CELL_PATH_W);
      }
      if(Maze::CELL_END == (tile & Maze::CELL_END))
      {
         //
         this->endFoundChangeToBlue = true;
         //return rVMarker;
      }

      int tempX = this->pos.first;
      int tempY = this->pos.second;
      bool deadEnd = false;

      if (road.size() == 1 && this->first == false) 
      {
         deadEnd = true;
      }

      
      do
      {
         tempX = this->pos.first;
         tempY = this->pos.second;

         int i = randGen.rand(0, road.size() - 1);
         tile = road[i];

         if(Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
         {
            tempY += -1;
         }
         if(Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
         {
            tempY += 1;
         }
         if(Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
         {
            tempX += 1;
         }
         if(Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
         {
            tempX += -1;
         }

         if (deadEnd)
         {
            this->changeNextYellowToRed = true;
         }
         
      } while(false == endFoundChangeToBlue && false == deadEnd && tempX == this->lastPos.first && tempY == this->lastPos.second);


      this->lastPos.first = this->pos.first;
      this->lastPos.second = this->pos.second;

      this->pos.first = tempX;
      this->pos.second = tempY;


      if(road.size() >= 3)
      {
         rVMarker = {Marker::MarkerType::NOT_FULLY_DISCOVER_PATH, this->pos, this->lastPos };
      }

      if (changeNextYellowToRed == true)
      {
         for (const auto& mark : surrMarkers)
         {
            if (mark.getX() == this->pos.first && mark.getY() == this->pos.second)
            {
               rVMarker = { Marker::MarkerType::CLOSED_PATH, this->pos, this->lastPos };
               changeNextYellowToRed = false;
            }
            else if (mark.getX() == this->lastPos.first && mark.getY() == this->lastPos.second)
            {
               rVMarker = { Marker::MarkerType::CLOSED_PATH, this->lastPos, this->pos };
               changeNextYellowToRed = false;
            }
         }
      }
     
      first = false;
   }
   else
   {
      if (false == this->finishedMaze)
      {
         if (false == this->blueChangedGoToEnd)
         {
            for (const auto& mark : surrMarkers)
            {
               if (mark.getX() == this->pos.first && mark.getY() == this->pos.second)
               {
                  this->blueChangedGoToEnd = true;
                  rVMarker = { Marker::MarkerType::PATH_TO_EXIT, this->pos, this->lastPos };
               }
            }
            std::vector<int> road;
            if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
            {
               road.push_back(Maze::CELL_PATH_N);
            }
            if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
            {
               road.push_back(Maze::CELL_PATH_S);
            }
            if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
            {
               road.push_back(Maze::CELL_PATH_E);
            }
            if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
            {
               road.push_back(Maze::CELL_PATH_W);
            }
            if (Maze::CELL_END == (tile & Maze::CELL_END))
            {
               //
               this->endFoundChangeToBlue = true;
               //return rVMarker;
            }
            int tempX = this->pos.first;
            int tempY = this->pos.second;
            bool deadEnd = false;
            if (road.size() == 1 && this->first == false) {
               deadEnd = true;
            }
            do
            {
               tempX = this->pos.first;
               tempY = this->pos.second;

               int i = randGen.rand(0, road.size() - 1);
               tile = road[i];

               if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
               {
                  tempY += -1;
               }
               if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
               {
                  tempY += 1;
               }
               if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
               {
                  tempX += 1;
               }
               if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
               {
                  tempX += -1;
               }

            } while (false == endFoundChangeToBlue && false == deadEnd && tempX == this->lastPos.first && tempY == this->lastPos.second);

            this->lastPos.first = this->pos.first;
            this->lastPos.second = this->pos.second;

            this->pos.first = tempX;
            this->pos.second = tempY;

         }
         else
         {
            std::vector<int> road;
            if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
            {
               road.push_back(Maze::CELL_PATH_N);
            }
            if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
            {
               road.push_back(Maze::CELL_PATH_S);
            }
            if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
            {
               road.push_back(Maze::CELL_PATH_E);
            }
            if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
            {
               road.push_back(Maze::CELL_PATH_W);
            }
            if (Maze::CELL_END == (tile & Maze::CELL_END))
            {
               
               this->finishedMaze = true;
               return rVMarker;
            }
            int tempX = this->pos.first;
            int tempY = this->pos.second;
            bool deadEnd = false;
            if (road.size() == 1 && this->first == false) {
               deadEnd = true;
            }
            do
            {
               tempX = this->pos.first;
               tempY = this->pos.second;

               int i = randGen.rand(0, road.size() - 1);
               tile = road[i];

               if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
               {
                  tempY += -1;
               }
               if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
               {
                  tempY += 1;
               }
               if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
               {
                  tempX += 1;
               }
               if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
               {
                  tempX += -1;
               }

            } while (false == endFoundChangeToBlue && false == deadEnd && tempX == this->lastPos.first && tempY == this->lastPos.second);

            this->lastPos.first = this->pos.first;
            this->lastPos.second = this->pos.second;

            this->pos.first = tempX;
            this->pos.second = tempY;
         }
      }
   }


   return rVMarker;
}


int Ant::getID() const
{
   return this->id;
}

int Ant::getX() const
{
   return this->pos.first;
}

int Ant::getY() const
{
   return this->pos.second;
}

std::pair<int, int> Ant::getPosition() const
{
   return this->pos;
}

bool Ant::isFinishedMaze() const
{
   return this->finishedMaze;
}
