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

   //ten warunek jest po to, ze kiedy znajdziemy wyjscie, musimy zawrocic
   //(musimy oznaczyc sciezke z ktorej przyszlismy)
   if(this->endFoundChangeToBlue)
   {
      choosenPos = this->lastPos;
      return choosenPos;
   }

   //przyjecie jedynej drogi przy trafieniu na slepy zaulek
   if(road.size() == 1 && this->first == false)
   {
      int tile = road[0];
      tileCheck(tile, choosenPos.first, choosenPos.second);
      return choosenPos;
   }
   //zmienne do zliczania poszczegolnych rodzajow pol
   int redCount = 0;
   int yellowCount = 0;
   int emptyCount = 0;
   //dla kazdego kierunku w road
   for(auto& r : road)
   {
      temp = this->pos;
      //w road sa tylko i wylacznie KIERUNKI, a my potrzebujemy
      //PUNKTOW, tileCheck to pozamienia
      tileCheck(r, temp.first, temp.second);
      //w tej chwili wydaje mi sie ze ify przed ta petla
      //zawieraja jedyne sytuacje w ktorych musimy "zawrocic"
      //skoro sa przed ta petla, to kierunek skad przyszlismy mozna wywalic zupelnie 
      //z nastepnych dzialan
      if (temp == this->lastPos) 
      {
         continue;
      }
      bool isRoadMarked = false;
      for(const auto& mark : surrMarkers)
      {
         //jesli na polu jest marker, sprawdzamy jakiego rodzaju
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
      //jesli flaga sie nie odpalila, na polu nie ma zadnego markera
      if(isRoadMarked == false)
      {
         emptyCount++;
      }

   }

   //GDZIES W TYM IFIE MUSI BYC BLAD
   //wychodzil sobie ze sciezki i mial cos takiego
   //  __|
   //  __ x
   //    |
   // i stal sobie w tym miejscu i nie mogl isc dalej z jakiegos powodu
   //raczej musi byc tu, bo tu jest wybierany ruch dla empty

   //pierwszenstwo wedlug algorytmu maja pola puste
   if(emptyCount > 0)
   {
      //jesli jest wiecej mozliwych kierunkow, trzeba wulosowac iterator
      //nie wiem czy dziala dla randGen.rand(0,0), wiec trzasne na to ifa

      int i;
      if (emptyCount == 1)
      {
         i = 0;
      }
      else
      {
         randGen.rand(0, emptyCount - 1);
      }
      
      int count = 0;
      bool found = false;

      for(auto& r : road)
      {
         temp = this->pos;
         tileCheck(r, temp.first, temp.second);
         //jak bylo wyzej, jesli natniemy sie w road na cofajacy kierunek
         //musimy to zignorowac, bo i tak nie jest ujete w liczniku
         if (temp == this->lastPos)
         {
            continue;
         }
         bool isMarked = false;
         //z powyzszych dzialan dostajemy tylko liczebnosc poszczegolnych
         //rodzajow pol, a musimy wybrac pozycje, wiec przebiegamy przez wszystko
         //jeszcze raz

         //tutaj patrze czy obecnie sprawdzane pole kawlifikuje sie do
         //dalszych dzialan, patrz --> nie ma na sobie markerow
         for(const auto& mark : surrMarkers)
         {
            if(mark.getPos() == temp)
            {
               isMarked = true;

            }
         }
         //isMarked sprawdza czy na danym polu jest marker
         //found jest troche chore --> blokuje mi ifa jesli juz znajde wylosowane pole
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
      //inna opcja wykrycia utkniecia w zaulku, 
      //sprawdzam czy jedyna opcja zamiast cofniecia jest przejazd przez czerwona kropke

      //jako ze to sytuacja wyjatkowa, moze byc (CHYBA) przed sprawdzeniem zoltych
      //normalnie zolte maja pierwszenstwo z zasady, bo na czerwone nie mozna wchodzic -_-
      if(road.size() == 2)
      {
         temp = this->pos;
         temp1 = this->pos;
         tileCheck(road[0], temp.first, temp.second);
         tileCheck(road[1], temp1.first, temp1.second);
         //no wiec skoro mamy dwa pola, to sytuacja sie zgadza jesli na jednym
         //jest czerwony marker, a na drugim miejsce skad przyszlismy
         for(const auto& mark : surrMarkers)
         {
            if(mark.CLOSED_PATH == Marker::CLOSED_PATH && mark.getPos() == temp)
            {
               if(temp1 == this->lastPos)
               {
                  choosenPos = temp;
               }
            }
            else if (mark.CLOSED_PATH == Marker::CLOSED_PATH && mark.getPos() == temp1)
            {
               if (temp == this->lastPos)
               {
                  choosenPos = temp1;
               }
            }
         }
      }
   }
   else
   {
      //zasadniczo to samo co w empty
      int i;
      if (emptyCount == 1)
      {
         i = 0;
      }
      else
      {
         randGen.rand(0, emptyCount - 1);
      }

      int count = 0;
      bool found = false;

      for (auto& r : road)
      {
         temp = this->pos;
         tileCheck(r, temp.first, temp.second);
         if (temp == this->lastPos)
         {
            continue;
         }

         for (const auto& mark : surrMarkers)
         {
            //tutaj tylko patrzymy czy jest odpowiedni marker, zamiast, czy nie ma w ogole
            if (mark.getPos() == temp && mark.NOT_FULLY_DISCOVER_PATH == Marker::NOT_FULLY_DISCOVER_PATH)
            {
               if (count == i && found == false)
               {
                  choosenPos = temp;
                  found = true;
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
