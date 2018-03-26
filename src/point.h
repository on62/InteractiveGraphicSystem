#ifndef GTKMM_APP_POINT
#define GTKMM_APP_POINT

#include "drawableobject.h"
#include "coordinate.h"

class Point : public DrawableObject
{
public:
  Point(std::string name, Coordinate* point_cord)
      : DrawableObject(name)
  {
    coordinates.push_back(point_cord);
  }

  ~Point()
  {
  }
};
#endif // GTKMM_APP_POINT
