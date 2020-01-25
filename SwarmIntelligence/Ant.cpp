#include "Ant.h"
#include "Maze.h"
#include <QDebug>

RandGen<std::knuth_b>Ant::randGen = RandGen<std::knuth_b>(time(NULL));

Ant::Ant(int _id, int _x, int _y)
   : id(_id), pos(_x, _y), lastPos(_x, _y)
{
}

std::optional<Marker> Ant::update(int tile, const std::vector<Marker>& surrMarkers, const Marker& presentPosMarker)
{
   std::optional<Marker> rVMarker = std::nullopt;


   if (false == this->endFoundChangeToBlue)
   {
      //get possible directions
      std::vector<int> road;
      roadCheck(road, tile);

      //check if its a end
      if (Maze::CELL_END == (tile & Maze::CELL_END))
      {
         this->endFoundChangeToBlue = true;
      }

      if (road.size() == 1 && this->first == false)
      {
         this->changeNextYellowToRed = true;
      }
      
      auto tempPos = chooseNextPos(road, surrMarkers);

      this->lastPos = this->pos;
      this->pos = tempPos;

      if (road.size() >= 3)
      {
         rVMarker = { Marker::MarkerType::NOT_FULLY_DISCOVER_PATH, this->pos, this->lastPos };
      }

      if (this->changeNextYellowToRed == true)
      {
         for (const auto& mark : surrMarkers)
         {
            //jesli pozycja na ktora weszlismy ma zolty marker, zamieniamy na czerwony
            if (mark.getPos() == this->pos && mark.getType() == Marker::NOT_FULLY_DISCOVER_PATH)
            {
               rVMarker = { Marker::MarkerType::CLOSED_PATH, this->pos, this->lastPos };
               changeNextYellowToRed = false;
            }
         }
         //warunek na jedokafelkowy slepy zaulek?
         if (presentPosMarker.getPos() == this->lastPos && presentPosMarker.getType() == Marker::NOT_FULLY_DISCOVER_PATH)
         {
            rVMarker = { Marker::MarkerType::CLOSED_PATH, this->lastPos, this->lastPos };
            changeNextYellowToRed = false;
         }
      }

      this->first = false;
   }
   else
   {
      if (false == this->finishedMaze)
      {
         //dopoki nie zmienimy ostatniego markera na niebieski
         if (false == this->blueChangedGoToEnd)
         {
            //warunek sprawdzajacy czy jest tu marker
            //jesli tak to zamien go na niebieski
            if (presentPosMarker.getPos() == this->pos)
            {
               this->blueChangedGoToEnd = true;
               rVMarker = { Marker::MarkerType::PATH_TO_EXIT, this->pos, this->lastPos };
            }

            //wektor mozliwych kierunkow
            std::vector<int> road;
            roadCheck(road, tile);

            //TODO
            //Na tym etapie mamy tylko dwie mozliwosci tak naprawde:
            //1. Cofnac sie -> robimy to kiedy nie znalezlismy jeszcze markera
            //2. Isc dalej, czyli inaczej, po prostu sie nie cofac
            //Ponizszy fragment kodu mozna wiec pewnie baaardzo uproscic, ale technicznie
            //rzecz biorac dziala, wiec na razie zostawiam
            
            //potrzebujemy boola przetrzymujacego informacje, czy musimy losowac
            //pozycje jeszcze raz
            bool oneMoreTime = false;
            auto tempPos = this->pos; 
            do
            {
               tempPos = this->pos;
               //losujemy kierunek
               int i = randGen.rand(0, road.size() - 1);
               tile = road[i];
               tileCheck(tile, tempPos.first, tempPos.second);
               //jesli nie zmienilismy jeszcze kropki, ale wylosowalo nam 
               //cofniecie to jest zle
               if (this->blueChangedGoToEnd == false && tempPos == this->lastPos)
               {
                  oneMoreTime = true;
               }
               else
               {
                  oneMoreTime = false;
               }
               //za to jesli w tej kolejce zmienilismy kropke, musimy sie cofnac
               if (this->blueChangedGoToEnd == true)
               {
                  tempPos = this->lastPos;
                  oneMoreTime = false;
               }
            } while (oneMoreTime);
            
            this->lastPos = this->pos;
            this->pos = tempPos;
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
            auto tempPos = this->pos;
            bool deadEnd = false;

            if (road.size() == 1 && this->first == false)
            {
               deadEnd = true;
            }
            do
            {
               tempPos = this->pos;

               int i = randGen.rand(0, road.size() - 1);
               tile = road[i];

               tileCheck(tile, tempPos.first, tempPos.second);

            } while (false == endFoundChangeToBlue &&
               false == deadEnd &&
               tempPos == this->lastPos);

            this->lastPos = this->pos;
            this->pos = tempPos;
         }
      }
   }


   return rVMarker;
}

std::pair<int, int> Ant::chooseNextPos(std::vector<int>& road, const std::vector<Marker>& surrMarkers)
{
   auto choosenPos = this->pos;
   auto tempPos = this->pos;
   auto temp1 = this->pos;

   //ten warunek jest po to, ze kiedy znajdziemy wyjscie, musimy zawrocic
   //(musimy oznaczyc sciezke z ktorej przyszlismy)
   if (this->endFoundChangeToBlue)
   {
      choosenPos = this->lastPos;
      return choosenPos;
   }

   //przyjecie jedynej drogi przy trafieniu na slepy zaulek
   if (road.size() == 1 && false == this->first)
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
   for (auto& r : road)
   {
      tempPos = this->pos;
      //w road sa tylko i wylacznie KIERUNKI, a my potrzebujemy
      //PUNKTOW, tileCheck zamienia dany KIERUNEK na pozycjê
      tileCheck(r, tempPos.first, tempPos.second);
      //w tej chwili wydaje mi sie ze ify przed ta petla
      //zawieraja jedyne sytuacje w ktorych musimy "zawrocic"
      //skoro sa przed ta petla, to kierunek skad przyszlismy mozna wywalic zupelnie 
      //z nastepnych dzialan
      if (tempPos == this->lastPos)
      {
         continue;
      }

      bool isRoadMarked = false;
      for (const auto& mark : surrMarkers)
      {
         //jesli na polu jest marker, sprawdzamy jakiego rodzaju
         if (mark.getPos() == tempPos)
         {
            isRoadMarked = true;
            if (mark.getType() == Marker::PATH_TO_EXIT)
            {
               choosenPos = tempPos;
               return choosenPos;
            }
            else if (mark.getType() == Marker::CLOSED_PATH)
            {
               redCount++;
            }
            else if (mark.getType() == Marker::NOT_FULLY_DISCOVER_PATH)
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
      if (isRoadMarked == false)
      {
         emptyCount++;
      }

   }
   qDebug() << "EC: " << emptyCount << "YC: " << yellowCount << "  RC: " << redCount;
   //pierwszenstwo wedlug algorytmu maja pola puste
   if (emptyCount > 0)
   {
      //jesli jest wiecej mozliwych kierunkow, trzeba wulosowac iterator
      //nie wiem czy dziala dla randGen.rand(0,0), wiec trzasne na to ifa

      int i = randGen.rand(0, emptyCount - 1);


      int count = 0;
      bool found = false;

      for (auto& r : road)
      {
         tempPos = this->pos;
         tileCheck(r, tempPos.first, tempPos.second);
         //jak bylo wyzej, jesli natniemy sie w road na cofajacy kierunek
         //musimy to zignorowac, bo i tak nie jest ujete w liczniku
         if (tempPos == this->lastPos)
         {
            continue;
         }
         bool isMarked = false;
         //z powyzszych dzialan dostajemy tylko liczebnosc poszczegolnych
         //rodzajow pol, a musimy wybrac pozycje, wiec przebiegamy przez wszystko
         //jeszcze raz

         //tutaj patrze czy obecnie sprawdzane pole kawlifikuje sie do
         //dalszych dzialan, patrz --> nie ma na sobie markerow
         for (const auto& mark : surrMarkers)
         {
            if (mark.getPos() == tempPos)
            {
               isMarked = true;

            }
         }
         //isMarked sprawdza czy na danym polu jest marker
         //found jest troche chore --> blokuje mi ifa jesli juz znajde wylosowane pole
         if (isMarked == false && found == false)
         {
            if (count == i)
            {
               choosenPos = tempPos;
               found = true;
            }
            else
            {
               count++;
            }
         }
      }

   }
   else if (redCount == 1 && road.size() == 2)
   {
      //inna opcja wykrycia utkniecia w zaulku, 
      //sprawdzam czy jedyna opcja zamiast cofniecia jest przejazd przez czerwona kropke

      //jako ze to sytuacja wyjatkowa, moze byc (CHYBA) przed sprawdzeniem zoltych
      //normalnie zolte maja pierwszenstwo z zasady, bo na czerwone nie mozna wchodzic -_-

      tempPos = this->pos;
      temp1 = this->pos;
      tileCheck(road[0], tempPos.first, tempPos.second);
      tileCheck(road[1], temp1.first, temp1.second);
      //no wiec skoro mamy dwa pola, to sytuacja sie zgadza jesli na jednym
      //jest czerwony marker, a na drugim miejsce skad przyszlismy
      for (const auto& mark : surrMarkers)
      {
         if (mark.getType() == Marker::CLOSED_PATH && mark.getPos() == tempPos)
         {
            if (temp1 == this->lastPos)
            {
               choosenPos = tempPos;
            }
         }
         else if (mark.getType() == Marker::CLOSED_PATH && mark.getPos() == temp1)
         {
            if (tempPos == this->lastPos)
            {
               choosenPos = temp1;
            }
         }
      }

   }
   else if (yellowCount > 0)
   {
      //zasadniczo to samo co w empty
      int i = randGen.rand(0, yellowCount - 1);


      int count = 0;
      bool found = false;

      for (auto& r : road)
      {
         tempPos = this->pos;
         tileCheck(r, tempPos.first, tempPos.second);
         if (tempPos == this->lastPos)
         {
            continue;
         }

         for (const auto& mark : surrMarkers)
         {
            //tutaj tylko patrzymy czy jest odpowiedni marker, zamiast, czy nie ma w ogole
            if (mark.getPos() == tempPos && mark.getType() == Marker::NOT_FULLY_DISCOVER_PATH)
            {
               if (count == i && found == false)
               {
                  choosenPos = tempPos;
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
