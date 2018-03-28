#include "drawableobject.h"

DrawableObject::DrawableObject(std::string name) :
      name(name)
{
}

DrawableObject::DrawableObject(std::string name, std::list<Coordinate*> coordinates) :
      name(name),
      coordinates(coordinates)
{
  LOG(4, "Deep coping initializing the other attributes");

  for( auto coordinate : coordinates )
  {
    this->viewWindowCoordinates.push_back(new Coordinate(*coordinate));
    this->clipped_coordinates.push_back(new Coordinate(*coordinate));
  }
}

DrawableObject::~DrawableObject()
{
	this->destroyList(coordinates);
}

/**
 * https://stackoverflow.com/questions/307082/cleaning-up-an-stl-list-vector-of-pointers
 * Cleaning up an STL list/vector of pointers
 */
void DrawableObject::destroyList(std::list<Coordinate*> coordinates)
{
  while(!coordinates.empty())
  {
    // delete coordinates.front();
    coordinates.pop_front();
  }
}

void DrawableObject::updateClipping(ViewPort& axes)
{
  LOG(4, "Generic clipping update... %s", axes);
  this->clipped_coordinates = this->coordinates;
}

std::string DrawableObject::getName()
{
  return this->name;
}

std::list<Coordinate*>& DrawableObject::getCoordinates()
{
  return this->coordinates;
}

std::list<Coordinate*>& DrawableObject::getviewWindowCoordinates()
{
  return this->viewWindowCoordinates;
}

void DrawableObject::setviewWindowCoordinates(std::list<Coordinate*> coordinates)
{
  this->destroyList(this->viewWindowCoordinates);
  this->coordinates= coordinates;
}

void DrawableObject::setCoordinates(std::list<Coordinate*> coordinates)
{
  this->destroyList(this->coordinates);
  this->coordinates= coordinates;
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

Coordinate* DrawableObject::get_window_geometric_center()
{
  auto coordinates      = this->getviewWindowCoordinates();
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

std::list<Coordinate*>& DrawableObject::getClippedCoordinates()
{
  return this->coordinates;
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
  std::list< std::list<Coordinate*> > coordinates_lists;

  coordinates_lists.push_back(this->coordinates);
  coordinates_lists.push_back(this->clipped_coordinates);
  coordinates_lists.push_back(this->viewWindowCoordinates);

  output << this->name;

  for( auto coordinates_list : coordinates_lists )
  {
    index = 0;
    size = coordinates_list.size() - 1;

    output << "-" << index << "(";

    for( auto coordinate : coordinates_list )
    {
      output << *coordinate;

      if( index != size )
      {
        output << ", ";
      }

      index++;
    }

    output << ")";
  }
}

void DrawableObject::apply(Transformation &transformation)
{
  LOG(8, "Entering apply");
  auto coordinates = this->getCoordinates();
  auto geometric_center = this->get_geometric_center();

  transformation.set_geometric_center(*geometric_center);
  delete geometric_center;

  for(auto coordinate : coordinates)
  {
    transformation.apply(*coordinate);
  }
}

void DrawableObject::applyInWindow(Transformation &transformation)
{

  LOG(8, "Entering applyInWindow");
  auto coordinates = this->getviewWindowCoordinates();
  auto geometric_center = this->get_window_geometric_center();

  transformation.set_geometric_center(*geometric_center);
  delete geometric_center;

  for(auto coordinate : coordinates)
  {
    transformation.apply(*coordinate);
  }

}
