#include "Marker.h"

Marker::Marker(uint32_t x, uint32_t y)
{
   this->x = x;
   this->y = y;

   this->type = NOT_FULLY_DISCOVER_PATH;
}

std::pair<uint32_t, uint32_t> Marker::getCoordinates() const
{
   return {this->x, this->y};
}

uint32_t Marker::getX() const
{
   return this->x;
}

uint32_t Marker::getY() const
{
   return this->y;
}

QColor Marker::getColor() const
{
   if(this->type == MarkerType::NOT_FULLY_DISCOVER_PATH)
   {
      return Qt::blue;
   }
   return QColor();
}
