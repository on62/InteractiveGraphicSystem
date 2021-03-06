#ifndef GTKMM_APP_POINT
#define GTKMM_APP_POINT

#include "drawableobject.h"
#include "coordinate.h"

class Point : public DrawableObject
{
public:
  Point(std::string name, Coordinate* point_cord, Coordinate _borderColor);
  ~Point();

  virtual void updateClippingCoordinates(const Axes&);
};
#endif // GTKMM_APP_POINT
