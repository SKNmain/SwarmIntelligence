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
      //get possible directions
      std::vector<int> road;
      roadCheck(road, tile);

      //check if its a end
      if(Maze::CELL_END == (tile & Maze::CELL_END))
      {
         this->endFoundChangeToBlue = true;
         //return rVMarker;
      }

      int tempX = this->pos.first;
      int tempY = this->pos.second;
      bool deadEnd = false;

      if(road.size() == 1 && this->first == false)
      {
         deadEnd = true;
         this->changeNextYellowToRed = true;
      }

      //zamiast calej tej petli musi byc funkcja wyboru
      std::pair<int, int> temp = this->pos;
      temp = chooseNextPos(road, surrMarkers);
      /*do
      {
         tempX = this->pos.first;
         tempY = this->pos.second;

         int i = randGen.rand(0, road.size() - 1);
         //tu musi isc

         tile = road[i];

         tileCheck(tile, tempX, tempY);

         if (deadEnd)
         {
            this->changeNextYellowToRed = true;
         }

      } while(false == endFoundChangeToBlue && false == deadEnd && (tempX == this->lastPos.first && tempY == this->lastPos.second));
      */

      this->lastPos = this->pos;

      this->pos = temp;


      if(road.size() >= 3)
      {
         rVMarker = { Marker::MarkerType::NOT_FULLY_DISCOVER_PATH, this->pos, this->lastPos };
      }

      if(this->changeNextYellowToRed == true)
      {
         for(const auto& mark : surrMarkers)
         {
            if(mark.getPos() != this->pos)
            {
               rVMarker = { Marker::MarkerType::CLOSED_PATH, this->pos, this->lastPos };
               changeNextYellowToRed = false;
            }
            else if(mark.getPos() != this->lastPos)
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
      if(false == this->finishedMaze)
      {
         if(false == this->blueChangedGoToEnd)
         {
            for(const auto& mark : surrMarkers)
            {
               if(mark.getPos() != this->pos)
               {
                  this->blueChangedGoToEnd = true;
                  rVMarker = { Marker::MarkerType::PATH_TO_EXIT, this->pos, this->lastPos };
               }
            }
            std::vector<int> road;
            roadCheck(road, tile);
            if(Maze::CELL_END == (tile & Maze::CELL_END))
            {
               //
               this->blueChangedGoToEnd = true;
               //return rVMarker;
            }
            auto temp = this->pos;
            bool deadEnd = false;
            if(road.size() == 1 && this->first == false) {
               deadEnd = true;
            }
            do
            {
               auto temp = this->pos;

               int i = randGen.rand(0, road.size() - 1);
               tile = road[i];

               tileCheck(tile, temp.first, temp.second);

            } while(false == blueChangedGoToEnd && false == deadEnd && temp == this->lastPos);

            this->lastPos = this->pos;
            this->pos = temp;

         }
         else
         {
            std::vector<int> road;
            roadCheck(road, tile);
            if(Maze::CELL_END == (tile & Maze::CELL_END))
            {

               this->finishedMaze = true;
               return rVMarker;
            }
            auto temp = this->pos;
            bool deadEnd = false;
            if(road.size() == 1 && this->first == false) {
               deadEnd = true;
            }
            do
            {
               temp = this->pos;

               int i = randGen.rand(0, road.size() - 1);
               tile = road[i];

               tileCheck(tile, temp.first, temp.second);

            } while(false == endFoundChangeToBlue && false == deadEnd && temp == this->lastPos);

            this->lastPos = this->pos;
            this->pos = temp;
         }
      }
   }


   return rVMarker;
}

std::pair<int, int> Ant::chooseNextPos(std::vector<int>& road, const std::vector<Marker>& surrMarkers)
{
   auto choosenPos = this->pos;
   auto temp = this->pos;
   auto temp1 = this->pos;

   if(this->endFoundChangeToBlue)
   {
      choosenPos = this->lastPos;
      return choosenPos;
   }

   if(road.size() == 1 && this->first == false)
   {
      int tile = road[0];
      tileCheck(tile, choosenPos.first, choosenPos.second);
      return choosenPos;
   }

   int redCount = 0;
   int yellowCount = 0;
   int emptyCount = 0;

   for(auto& r : road)
   {
      temp = this->pos;
      tileCheck(r, temp.first, temp.second);
      bool isRoadMarked = false;
      for(const auto& mark : surrMarkers)
      {
         if(mark.getPos() == temp)
         {
            isRoadMarked = true;
            if(mark.PATH_TO_EXIT == Marker::PATH_TO_EXIT)
            {
               choosenPos = temp;
               return choosenPos;
            }
            else if(mark.CLOSED_PATH == Marker::CLOSED_PATH)
            {
               redCount++;
            }
            else if(mark.NOT_FULLY_DISCOVER_PATH == Marker::NOT_FULLY_DISCOVER_PATH)
            {
               yellowCount++;
            }
            //else
            //{
              // emptyCount++;
            //}
         }
      }
      if(isRoadMarked == false)
      {
         emptyCount++;
      }

   }

   if(emptyCount > 0)
   {
      int i = randGen.rand(0, emptyCount - 1);
      int count = 0;
      bool found = false;

      for(auto& r : road)
      {
         temp = this->pos;
         tileCheck(r, temp.first, temp.second);
         bool isMarked = false;
         for(const auto& mark : surrMarkers)
         {
            if(mark.getPos() == temp)
            {
               isMarked = true;

            }
         }
         if(isMarked == false && found == false)
         {
            if(count == i)
            {
               choosenPos = temp;
               found = true;
            }
            else
            {
               count++;
            }
         }
      }

   }
   else if(redCount == 1)
   {
      //inna opcja wykrycia utkniecia w zaulku, zobacz, czy dziala
      //sprawdzam czy jedyna opcja zamiast cofniecia jest przejazd przez czerwona kropke
      if(road.size() == 2)
      {
         temp = this->pos;
         temp1 = this->pos;
         tileCheck(road[0], temp.first, temp.second);
         tileCheck(road[1], temp1.first, temp1.second);
         for(const auto& mark : surrMarkers)
         {
            if(mark.CLOSED_PATH == Marker::CLOSED_PATH && mark.getPos() == temp)
            {
               if(temp1 == this->lastPos)
               {
                  choosenPos = temp;
               }
               else if(temp == this->lastPos)
               {
                  choosenPos = temp1;
               }
            }
         }
      }



   }
   else
   {
      int i = randGen.rand(0, yellowCount - 1);
      int count = 0;

      for(auto& r : road)
      {
         temp = this->pos;
         tileCheck(r, temp.first, temp.second);
         for(const auto& mark : surrMarkers)
         {
            if(mark.getPos() == temp && mark.NOT_FULLY_DISCOVER_PATH == Marker::NOT_FULLY_DISCOVER_PATH)
            {
               if(count == i)
               {
                  choosenPos = mark.getPos();
               }
               else {
                  count++;
               }
            }
         }
      }
   }

   return choosenPos;

}


void Ant::tileCheck(int tile, int& tempX, int& tempY)
{
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
}

void Ant::roadCheck(std::vector<int>& r, int tile)
{
   if(Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
   {
      r.push_back(Maze::CELL_PATH_N);
   }
   if(Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
   {
      r.push_back(Maze::CELL_PATH_S);
   }
   if(Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
   {
      r.push_back(Maze::CELL_PATH_E);
   }
   if(Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
   {
      r.push_back(Maze::CELL_PATH_W);
   }

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
