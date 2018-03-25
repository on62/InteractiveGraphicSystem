#include "drawableobject.h"

DrawableObject::DrawableObject(std::string name) :
      name(name)
{
}

DrawableObject::DrawableObject(std::string name, std::list<Coordinate*> coordinates) :
      name(name),
      coordinates(coordinates)
{
  this->updateClipping();
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::disconnectObserver()
{
  if( !this->_connection.disconnect() )
  {
    LOG(1, "");
    LOG(1, "");
    LOG(1, "ERROR! Could not disconnect the object `%s` from its observer.", *this);
  }
}

void DrawableObject::addConnection(Signal<>::Connection connection)
{
  this->_connection = connection;
}

void DrawableObject::updateClipping()
{
  LOG(4, "Updating clipping...");
  this->clipped_coordinates = this->coordinates;
}

std::string DrawableObject::getName()
{
  return this->name;
}

Coordinate* DrawableObject::get_geometric_center()
{
  auto coordinates      = this->getCoordinates();
  int coordinates_count = coordinates.size();

  long int x_axis = 0;
  long int y_axis = 0;
  long int z_axis = 0;

  for(auto coordinate : coordinates)
  {
    x_axis += coordinate->getx();
    y_axis += coordinate->gety();
    z_axis += coordinate->getz();
  }

  return new Coordinate(x_axis/coordinates_count, y_axis/coordinates_count, z_axis/coordinates_count);
}

std::list<Coordinate*>& DrawableObject::getCoordinates()
{
  return this->coordinates;
}

std::list<Coordinate*>& DrawableObject::getClippedCoordinates()
{
  return this->coordinates;
}

/**
 * Prints a more beauty version of the object when called on `std::cout<< object << std::end;`
 */
std::ostream& operator<<( std::ostream &output, const DrawableObject &object )
{
  output << object.name << "(";

  unsigned int index = 0;
  unsigned int size = object.coordinates.size() - 1;

  for( auto coordinate : object.coordinates )
  {
    output << *coordinate;

    if( index != size )
    {
      output << ", ";
    }

    index++;
  }

  output << ")";
  return output;
}

void DrawableObject::apply(Transformation &transformation)
{
  auto coordinates = this->getCoordinates();
  auto geometric_center = this->get_geometric_center();

  transformation.set_geometric_center(*geometric_center);
  delete geometric_center;

  for(auto coordinate : coordinates)
  {
    transformation.apply(*coordinate);
  }

}
