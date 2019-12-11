#pragma once
#include <qpair.h>
#include <QColor>

class Marker
{
public:
   enum MarkerType
   {
      CLOSED_PATH,
      PATH_TO_EXIT,
      NOT_FULLY_DISCOVER_PATH,
      GEN_MAP_TAG,
   };
   Marker(MarkerType type, const std::pair<int, int>& pos = { 0, 0 }, const std::pair<int, int>& enterPos = { 0, 0 });


   std::pair<int, int> getPos() const;
   std::pair<int, int> getEnterPos() const;
   int getX() const;
   int getY() const;

   MarkerType getType() const;
   
   QColor getColor() const;

private:
   std::pair<int, int> pos;
   //<Position of ant before this marker was placed
   std::pair<int, int> enterPos;
   MarkerType type;
};

