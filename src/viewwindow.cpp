#include "viewwindow.h"

ViewWindow::ViewWindow() :
      _axes(ViewWindow::xWiMin, ViewWindow::yWiMin, ViewWindow::xWiMax, ViewWindow::yWiMax),
      _angles(0, 0, 0),
      _dimentions(100, 100, 0),
      _windowCenter(0, 0, 0),
      _projection(Projection::PARALLEL),
      _projectionDistance(0.0)
{
  this->reset(false);
}

ViewWindow::~ViewWindow()
{
}

void ViewWindow::reset(bool isToCallObservers) {
  _angles = *new Coordinate(15, 15, 0);
  _dimentions = *new Coordinate(100, 100, 0);
  _windowCenter = *new Coordinate(0, 0, 0);
  _projection = Projection::PARALLEL;
  _projectionDistance = 0.0;

  if(isToCallObservers) {
    this->callObservers();
  }
}

ViewWindow::UpdateViewWindowTitle::Connection ViewWindow::addObserver(const ViewWindow::UpdateViewWindowTitle::Callback& callback) {
  auto connection = this->_updateViewWindowTitle.connect(callback);
  return connection;
}

ViewWindow::UpdateAllObjectCoordinates::Connection ViewWindow::addObserver(const ViewWindow::UpdateAllObjectCoordinates::Callback& callback) {
  auto connection = this->_updateAllObjectCoordinates.connect(callback);
  return connection;
}

ViewWindow::UpdateObjectCoordinates::Connection ViewWindow::addObserver(const ViewWindow::UpdateObjectCoordinates::Callback& callback) {
  auto connection = this->_updateObjectCoordinates.connect(callback);
  return connection;
}

void ViewWindow::updateObjectCoordinates(DrawableObject* object) {
  LOG( 2, "..." );
  this->_updateObjectCoordinates( object, _getTransformation(), this->_axes );
}

std::ostream& operator<<( std::ostream &output, const ViewWindow &object )
{
  output << "ViewWindow" << object._dimentions << object._windowCenter << object._angles;
  return output;
}

void ViewWindow::zoom(Coordinate steps)
{
  Coordinate reversed = steps / 100.0;
  Coordinate new_values = _dimentions * reversed;

  LOG( 8, "steps: %s", steps );
  LOG( 8, "reversed: %s", reversed );
  LOG( 8, "new_values: %s", new_values );
  LOG( 8, "_dimentions: %s", _dimentions );

  if( (this->_dimentions[0] + new_values[0] < MINIMUM_ZOOM_LIMIT
      || this->_dimentions[1] + new_values[0] < MINIMUM_ZOOM_LIMIT)
    && new_values[0] < 0 )
  {
    std::string error = tfm::format( "You reached the minimum zoom limit! %s", MINIMUM_ZOOM_LIMIT );
    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }
  else if( (this->_dimentions[0] + new_values[0] > MAXIMUM_ZOOM_LIMIT
            || this->_dimentions[1] + new_values[0] > MAXIMUM_ZOOM_LIMIT)
          && new_values[0] > 0)
  {
    std::string error = tfm::format( "You reached the maximum zoom limit! %s", MAXIMUM_ZOOM_LIMIT );
    LOG( 1, "%s", error );
    throw std::runtime_error( error );
  }
  else {
    this->_dimentions += new_values;
    this->callObservers();
  }
}

void ViewWindow::move(Coordinate moves)
{
  Transformation transformation;
  transformation.add_rotation("Fix window rotation", -this->_angles);
  transformation.set_geometric_center(_origin_coordinate_value);
  transformation.apply(moves);

  LOG(16, "moves: %s", moves);
  // LOG(4, "_angles: %s", this->_angles);
  // LOG(4, "moves transformation: %s", transformation);

  this->_windowCenter += moves;
  this->callObservers();
}

void ViewWindow::rotate(Coordinate angles)
{
  normalize_angle(_angles, -angles);
  this->callObservers();
}

Transformation ViewWindow::_getTransformation() {
  Coordinate angles = this->_angles;
  Coordinate inverse{ 1.0 / _dimentions.x, 1.0 / _dimentions.y, 1.0 };

  big_double angle_z = angles.z;
  angles.z = 0.0;

  Transformation transformation;
  transformation.add_translation( "Window to center", -this->_windowCenter );
  transformation.add_rotation( "Window rotation", -angles );

  if( _projection == Projection::PERSPECTIVE ) {
    transformation.add_translation( "Window to center", Coordinate( 0, 0, _projectionDistance ) );
    // transformation.add_matrix( "Perspective Projection",
    //     {
    //       {1.0, 0.0, 0.0, 0.0},
    //       {1.0, 1.0, 0.0, 0.0},
    //       {1.0, 0.0, 0.0, 0.0},
    //       {1.0, 0.0, 1.0/_projectionDistance, 1},
    //     }
    //   );
    // transformation.add_rotation( "Window rotation", Coordinate( 0, 0, -angle_z ) );
    // transformation.add_scaling( "Window coordinate scaling", inverse );

    transformation.projectionDistance = _projectionDistance;
    transformation.posTransformation = new Transformation();
    transformation.posTransformation->add_rotation( "Window rotation", Coordinate( 0, 0, -angle_z ) );
    transformation.posTransformation->add_scaling( "Window coordinate scaling", inverse );
    transformation.posTransformation->set_geometric_center( _origin_coordinate_value );
  }
  else {
    transformation.add_rotation( "Window rotation", Coordinate( 0, 0, -angle_z ) );
    transformation.add_scaling( "Window coordinate scaling", inverse );
  }

  transformation.set_geometric_center( _origin_coordinate_value );

  // LOG(1, "World transformation: %s", transformation);
  // LOG(16, "Window dimensions: %s, inversed dimensions: %s", this->_dimentions, inverse);
  return transformation;
}

void ViewWindow::callObservers() {
  this->_updateAllObjectCoordinates( _getTransformation(), this->_axes );
  this->_updateViewWindowTitle(*this);
}

/**
 * TODO: Keep viewPort and viewWindow portions while resizing the viewPort.
 */
void ViewWindow::updateViewPortSize(big_double width, big_double height)
{
  LOG( 2, "..." );
  // LOG(4, "Current drawing area widget size %sx%s - %s", width, height, *this);

  // This is true only when you resize the you ViewWindow widget window
  if (this->xVpMax != width || this->yVpMax != height)
  {
    big_double widthDiff  = width  - (this->xVpMax - this->xVpMin);
    big_double heightDiff = height - (this->yVpMax - this->yVpMin);

    // Keep the window zoom size while resizing the window
    // // On the first time we run this algorithm, the xVpMax is set to zero. Therefore, we must
    // // to initialize this within the current size of the ViewWindow
    // if (this->xVpMax != 0)
    // {
    //   this->viewWindow.xMax = this->viewWindow.xMax + (this->viewWindow.xMax - this->viewWindow.xMin)
    //       * widthDiff / (this->xVpMax - this->xVpMin);
    // }
    // else
    // {
    //   this->viewWindow.xMax = widthDiff;
    // }

    // if (this->yVpMax != 0)
    // {
    //   this->viewWindow.yMin = this->viewWindow.yMin - (this->viewWindow.yMax - this->viewWindow.yMin)
    //       * (heightDiff / (this->yVpMax - this->yVpMin));
    // }
    // else
    // {
    //   // LOG(8, "If we exchange this `viewWindow.yMax` to `viewWindow.yMin` our world becomes up-side-down");
    //   this->viewWindow.yMax = heightDiff;
    // }

    this->xVpMax += widthDiff;
    this->yVpMax += heightDiff;

    this->callObservers();
    // LOG(8, "Leaving:  %s", *this);
  }
}

/**
 * Transformada de viewport - Slide 2 de "02 - Conceitos Básicos"
 *
 * Resize `viewwindow` when `viewport` is resized:
 * http://www.di.ubi.pt/~agomes/cg/teoricas/04e-windows.pdf
 *
 * A strategy of keeping proportions automatically between window and viewport.
 *
 * Window-Viewport Mapping, important conclusion: As the world window increases in size the image in
 * viewport decreases in size and vice-versa.
 *
 * The user may enlarge or reduce the size of a viewport with w pixels wide and h pixels high by
 * pulling away the right-bottom of its interface window.
 *
 * To avoid distortion, we must change the size of the world window accordingly.
 *
 * For that, we assume that the initial world window is a square with side length L.
 *
 * A possible solution is to change the world window whenever the viewport of
 * the interface window were changed.
 *
 * @param  coordinate a normalized coordinate as from -1 to 1
 * @return            the viewport coordinate as from 0 to 500
 */
Coordinate* ViewWindow::convertCoordinateToViewPort(const Coordinate &c) const
{
  // double     x=((c.x -wmin.x) / (wmax.x-wmin.x)) *_width;
  big_double xVp =((c.x - xWiMin) / (xWiMax - xWiMin)) * (xVpMax - xVpMin);

  // double    y = (1.0 - ((c.y -wmin.y) /(wmax.y -wmin.y))) *_height;
  big_double yVp = (1.0 - ((c.y - yWiMin) / (yWiMax - yWiMin))) * (yVpMax - yVpMin);

  // std::cout << "transformCoordinate: " << Coordinate(xVp, yVp);
  // std::cout << ", Original: " << coordinate << std::endl;
  // std::cout << ", wiMin x: " << xWiMin << ", y: " << yWiMin;
  // std::cout << ", wiMax x: " << xWiMax << ", y: " << yWiMax;
  // std::cout << ", _width x: " << xVpMax - xVpMin << ", _height: " << yVpMax - yVpMin << std::endl;
  return new Coordinate(xVp, yVp, 0);
}
