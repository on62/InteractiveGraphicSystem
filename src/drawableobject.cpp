#include "drawableobject.h"

DrawableObject::DrawableObject(std::string name, std::vector<Coordinate*> _worldCoordinates) :
      DrawableObject(name, _worldCoordinates, Coordinate(0, 0))
{
}

DrawableObject::DrawableObject(std::string name, std::vector<Coordinate*> _worldCoordinates, Coordinate _borderColor, Coordinate _fillingColor) :
      name(name),
      _isDrawable(false),
      _borderColor(_borderColor),
      _fillingColor(_fillingColor),
      _worldCoordinates(_worldCoordinates)
{
  LOG(4, "Deep coping initializing the coordinate attributes");

  for( auto coordinate : _worldCoordinates )
  {
    this->_windowCoordinates.push_back(new Coordinate(*coordinate));
    this->_clippingCoordinates.push_back(new Coordinate(*coordinate));
  }
}

DrawableObject::~DrawableObject()
{
  this->destroyList(this->_worldCoordinates);
  this->destroyList(this->_windowCoordinates);
	this->destroyList(this->_clippingCoordinates);
}

/**
 * https://stackoverflow.com/questions/307082/cleaning-up-an-stl-list-vector-of-pointers
 * Cleaning up an STL list/vector of pointers
 */
void DrawableObject::destroyList(std::vector<Coordinate*>& _worldCoordinates)
{
  while(!_worldCoordinates.empty())
  {
    // delete _worldCoordinates.front();
    _worldCoordinates.pop_back();
  }
}

std::string DrawableObject::getName() const
{
  return this->name;
}

const std::vector<Coordinate*>& DrawableObject::worldCoordinates() const
{
  return this->_worldCoordinates;
}

const std::vector<Coordinate*>& DrawableObject::windowCoordinates() const
{
  return this->_windowCoordinates;
}

const std::vector<Coordinate*>& DrawableObject::clippingCoordinates() const
{
  return this->_clippingCoordinates;
}

Coordinate DrawableObject::getGeometricCenter(const std::vector<Coordinate*>& coordinates)
{
  int coordinatesCount = coordinates.size();

  big_double x_axes = 0;
  big_double y_axes = 0;
  big_double z_axes = 0;

  for(auto coordinate : coordinates)
  {
    x_axes += coordinate->x;
    y_axes += coordinate->y;
    z_axes += coordinate->z;
  }

  return Coordinate(x_axes/coordinatesCount, y_axes/coordinatesCount, z_axes/coordinatesCount);
}

/**
 * Prints a more beauty version of the object when called on `std::cout<< object << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const DrawableObject &object )
{
  object.printMyself(output);
  return output;
}

void DrawableObject::printMyself(std::ostream& output) const
{
  unsigned int size;
  unsigned int index;
  unsigned int index_external = 0;
  std::vector< std::vector<Coordinate*> > coordinates_lists;

  coordinates_lists.push_back(this->_worldCoordinates);
  coordinates_lists.push_back(this->_windowCoordinates);
  coordinates_lists.push_back(this->_clippingCoordinates);

  output << this->name;

  for( auto coordinates_list : coordinates_lists )
  {
    index = 0;
    size = coordinates_list.size() - 1;

    output << "_" << index_external << "(";

    for( auto coordinate : coordinates_list )
    {
      output << *coordinate;

      if( index != size )
      {
        output << ", ";
      }

      index++;
    }

    index_external++;
    output << ")";
  }
}

void DrawableObject::apply(Transformation &transformation)
{
  LOG(8, "Entering... %s", transformation);
  auto coordinates = this->worldCoordinates();

  auto geometricCenter = DrawableObject::getGeometricCenter(coordinates);
  transformation.set_geometric_center(geometricCenter);

  for(auto coordinate : coordinates)
  {
    transformation.apply(*coordinate);
  }
}

void DrawableObject::updateWindowCoordinates(const Transformation& transformation)
{
  LOG(8, "Entering... %s", transformation);
  Coordinate* new_coordinate;

  auto coordinates = this->worldCoordinates();
  DrawableObject::destroyList(this->_windowCoordinates);

  for(auto coordinate : coordinates)
  {
    new_coordinate = new Coordinate(*coordinate);
    transformation.apply(*new_coordinate);
    this->_windowCoordinates.push_back(new_coordinate);
  }
}

void DrawableObject::updateClippingCoordinates(const Axes& axes)
{
  LOG(4, "Generic clipping update... %s", axes);
  this->_clippingCoordinates = this->_windowCoordinates;
}
