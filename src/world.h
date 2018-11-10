#ifndef GTKMM_APP_WORLD
#define GTKMM_APP_WORLD

#include "debugger.h"

#include "line.h"
#include "curve.h"
#include "point.h"
#include "polygon.h"

#include "displayfile.h"
#include "subject_controller.h"

class World
{
public:
  World();
  ~World();

  void addPoint(const std::string name, const int, const int, Coordinate _borderColor);
  void addLine(const std::string name, const int, const int, const int, const int,
      Coordinate _borderColor, LineClippingType type=LineClippingType::LIANG_BARSKY, bool visible_on_gui=true);

  void addPolygon(const std::string name, const std::vector<big_double>, Coordinate _borderColor, Coordinate _fillingColor, CurveType type);
  void addPolygon(const std::string name, const std::vector<Coordinate*>, Coordinate _borderColor, Coordinate _fillingColor, CurveType type);
  void removeObject(const std::string name);

  const DisplayFile& displayFile() const { return this->_polygons; }

  void draw_xy_axes();
  void apply(const std::string object_name, Transformation& matrices);

  void updateAllObjectCoordinates(const Transformation&, const Axes&);
  static void updateObjectCoordinates(DrawableObject*, const Transformation&, const Axes&);

  /**
   * Implementations types for the Observer Design Pattern with C++ 11 templates and function
   * pointers, instead of tight coupled inheritance.
   */
  typedef Signal<DrawableObject*> UpdateObjectCoordinates;
  UpdateObjectCoordinates::Connection addObserver(const UpdateObjectCoordinates::Callback&);

protected:
  // /**
  //  * List of references to display files lists.
  //  */
  // std::vector< std::reference_wrapper< DisplayFile > > _polygons;
  DisplayFile _polygons;
  UpdateObjectCoordinates _updateObjectCoordinates;

  // DisplayFile _lines;
  // DisplayFile _points;
  // DisplayFile _polygons;
};

#endif // GTKMM_APP_WORLD
