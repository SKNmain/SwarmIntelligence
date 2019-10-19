#pragma once
#include <qpair.h>
#include <QColor>

class Marker
{
public:
   Marker(uint32_t x, uint32_t y);

   enum MarkerType
   {
      CLOSED_PATH,
      NOT_FULLY_DISCOVER_PATH,
   };

   std::pair<uint32_t, uint32_t> getCoordinates() const;
   uint32_t getX() const;
   uint32_t getY() const;

   QColor getColor() const;

private:
   uint32_t x;
   uint32_t y;
   MarkerType type;
};

