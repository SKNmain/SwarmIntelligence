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
      roadCheck(road, tile);
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
         this->changeNextYellowToRed = true;
      }
      
      //zamiast calej tej petli musi byc funkcja wyboru
      std::pair<int, int> temp(this->pos.first, this->pos.second);
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

      this->lastPos.first = this->pos.first;
      this->lastPos.second = this->pos.second;

      this->pos.first = temp.first;
      this->pos.second = temp.second;


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
            roadCheck(road, tile);
            if (Maze::CELL_END == (tile & Maze::CELL_END))
            {
               //
               this->blueChangedGoToEnd = true;
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

               tileCheck(tile, tempX, tempY);

            } while (false == blueChangedGoToEnd && false == deadEnd && tempX == this->lastPos.first && tempY == this->lastPos.second);

            this->lastPos.first = this->pos.first;
            this->lastPos.second = this->pos.second;

            this->pos.first = tempX;
            this->pos.second = tempY;

         }
         else
         {
            std::vector<int> road;
            roadCheck(road, tile);
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

               tileCheck(tile, tempX, tempY);

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

void Ant::roadCheck(std::vector<int>& r, int tile)
{
   if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
   {
      r.push_back(Maze::CELL_PATH_N);
   }
   if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
   {
      r.push_back(Maze::CELL_PATH_S);
   }
   if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
   {
      r.push_back(Maze::CELL_PATH_E);
   }
   if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
   {
      r.push_back(Maze::CELL_PATH_W);
   }

}

std::pair<int, int> Ant::chooseNextPos(std::vector<int>& road, const std::vector<Marker>& surrMarkers)
{
   std::pair<int, int> choosenPos(this->pos.first, this->pos.second);
   std::pair<int, int> temp(this->pos.first, this->pos.second);
   std::pair<int, int> temp1(this->pos.first, this->pos.second);
   if (this->endFoundChangeToBlue)
   {
      choosenPos = this->lastPos;
      return choosenPos;
   }
   if (road.size() == 1 && this->first == false)
   {
      int tile = road[0];
      tileCheck(tile, choosenPos.first, choosenPos.second);
      return choosenPos;
   }
   int redCount = 0;
   int yellowCount = 0;
   int emptyCount = 0;

   for (auto& r : road)
   {
      temp = { this->pos.first, this->pos.second };
      tileCheck(r, temp.first, temp.second);
      bool isRoadMarked = false;
      for (const auto& mark : surrMarkers)
      {
         if (mark.getX() == temp.first && mark.getY() == temp.second)
         {
            isRoadMarked = true;
            if (mark.PATH_TO_EXIT == Marker::PATH_TO_EXIT)
            {
               choosenPos = temp;
               return choosenPos;
            }
            else if (mark.CLOSED_PATH == Marker::CLOSED_PATH)
            {
               redCount++;
            }
            else if (mark.NOT_FULLY_DISCOVER_PATH == Marker::NOT_FULLY_DISCOVER_PATH)
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

   if (emptyCount > 0)
   {
      int i = randGen.rand(0, emptyCount - 1);
      int count = 0;
      bool found = false;

      for (auto& r : road)
      {
         temp = { this->pos.first, this->pos.second };
         tileCheck(r, temp.first, temp.second);
         bool isMarked = false;
         for (const auto& mark : surrMarkers)
         {
            if (mark.getX() == temp.first && mark.getY() == temp.second)
            {
               isMarked = true;
               
            }
         }
         if(isMarked == false && found == false)
         {
            if (count == i)
            {
               choosenPos.first = temp.first;
               choosenPos.second = temp.second;
               found = true;
            }
            else 
            {
               count++;
            }
         }
      }

   }
   else if (redCount == 1)
   {
      //inna opcja wykrycia utkniecia w zaulku, zobacz, czy dziala
      //sprawdzam czy jedyna opcja zamiast cofniecia jest przejazd przez czerwona kropke
      if (road.size() == 2)
      {
         temp  = { this->pos.first, this->pos.second };
         temp1 = { this->pos.first, this->pos.second };
         tileCheck(road[0], temp.first, temp.second);
         tileCheck(road[1], temp1.first, temp1.second);
         for (const auto& mark : surrMarkers)
         {
            if (mark.getX() == temp.first && mark.getY() == temp.second
               && mark.CLOSED_PATH && temp1 == this->lastPos)
            {
               choosenPos = temp;
            }
            else if (mark.getX() == temp1.first && mark.getY() == temp1.second
               && mark.CLOSED_PATH && temp == this->lastPos)
            {
               choosenPos = temp1;
            }
         }
      }


      
   }
   else
   {
      int i = randGen.rand(0, yellowCount - 1);
      int count = 0;

      for (auto& r : road)
      {
         temp = { this->pos.first, this->pos.second };
         tileCheck(r, temp.first, temp.second);
         for (const auto& mark : surrMarkers)
         {
            if (mark.getX() == temp.first && mark.getY() == temp.second 
               && mark.NOT_FULLY_DISCOVER_PATH)
            {
               if (count == i)
               {
                  choosenPos.first = mark.getX();
                  choosenPos.second = mark.getY();
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
