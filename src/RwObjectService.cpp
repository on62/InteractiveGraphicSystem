/*
 * RwObjectService.cpp
 *
 *  Created on: 23 de mar de 2018
 *      Author: karla
 */

#include "RwObjectService.h"

RwObjectService::RwObjectService(Facade& facade) :
      facade(facade)
{
}

RwObjectService::~RwObjectService()
{
}

void RwObjectService::read(std::string file_path)
{
  std::string line;
  bool is_to_skip_new_line_fetch = false;

  bool is_there_new_lines;
  bool is_there_a_next_line;

  std::ifstream myfile(file_path);
  std::vector<big_double> coordinates_points;
  std::string name;

  // removes all comments and empty lines

  if( myfile.is_open() )
  {
    while( true )
    {
      if( is_to_skip_new_line_fetch ) is_to_skip_new_line_fetch = false;
          else is_there_new_lines = static_cast<bool>( getline( myfile, line ) );

      if( !is_there_new_lines ) break;

      // https://www.fileformat.info/format/wavefrontobj/egff.htm
      if( line.front() == 'o' )
      {
        name = line.substr(2, line.length());
      }
      else if( line.front() == 'v' )
      {
        std::vector<std::string> indexes = split(line, ' ');

        if( indexes.size() > 2 )
        {
          coordinates_points.push_back( atof( indexes[1].c_str() ) );
          coordinates_points.push_back( atof( indexes[2].c_str() ) );
          coordinates_points.push_back( atof( indexes[3].c_str() ) );
        }
        else
        {
          coordinates_points.push_back( atof( indexes[1].c_str() ) );
          coordinates_points.push_back( atof( indexes[2].c_str() ) );
          coordinates_points.push_back( 1.0 );
        }
      }
      else if( line.front() == 'p' )
      {
        std::vector<int> indexes;
        this->getLineIndexes( indexes, line );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates_points );
        this->facade.addPoint( name, vertexes[0]->x, vertexes[1]->y );
      }
      else if( line.front() == 'l' )
      {
        std::vector<int> indexes;
        this->getLineIndexes( indexes, line );

        do
        {
          is_there_new_lines = static_cast<bool>( getline( myfile, line ) );
          is_there_a_next_line = line.front() == 'l';

          if( !is_there_a_next_line ) break;
          this->getLineIndexes( indexes, line );

          is_to_skip_new_line_fetch = true;
          if( !is_there_new_lines ) break;
        }
        while( true );

        std::vector<Coordinate*> vertexes = this->getVertexes( indexes, coordinates_points );

        if( vertexes.size() < 3 )
        {
          this->facade.addLine( name, vertexes[0]->x, vertexes[0]->y, vertexes[1]->x, vertexes[1]->y );
        }
        else
        {
          this->facade.addPolygon( name, vertexes );
        }
      }
    }

    myfile.close();
  }
  else
  {
    std::string error = tfm::format( "ERROR! Could not open the file: %s", file_path );
    throw std::runtime_error( error );
  }
}

std::vector<int> RwObjectService::getLineIndexes(std::vector<int>& internal, std::string& line)
{
  std::vector<std::string> indexes = this->split( line, ' ' );

  std::reverse( indexes.begin(), indexes.end() );
  indexes.pop_back();

  while(!indexes.empty())
  {
    internal.push_back( atoi( indexes.back().c_str() ) );
    indexes.pop_back();
  }

  LOGL( 1, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 1, "%s, ", value );

  LOGLN( 1, "\n" );
  return internal;
}

std::vector<Coordinate*> RwObjectService::getVertexes(std::vector<int>& indexes, std::vector<big_double>& coordinates_points)
{
  int index;
  Coordinate* coordinate;

  std::vector<Coordinate*> internal;
  std::reverse( indexes.begin(), indexes.end() );

  while(!indexes.empty())
  {
    index = ( indexes.back() - 1 ) * 3;
    indexes.pop_back();

    LOG( 1, "Reading index %s from %s", index, index / 3 );
    LOG( 1, "Reading x value %s", coordinates_points[index] );
    LOG( 1, "Reading y value %s", coordinates_points[index+1] );
    LOG( 1, "Reading z value %s", coordinates_points[index+2] );

    coordinate = new Coordinate( coordinates_points[index], coordinates_points[index+1], coordinates_points[index+2] );
    internal.push_back( coordinate );
  }

  LOGL( 1, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 1, "%s, ", *value );

  LOGLN( 1, "\n" );
  return internal;
}

std::vector<std::string> RwObjectService::split(std::string& line, char delimiter)
{
  std::vector<std::string> internal;
  std::stringstream ss(line); // Turn the std::string into a stream.
  std::string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  LOGL( 1, "internal.size: %s, ", internal.size() );
  for( auto value : internal ) LOGLN( 1, "%s, ", value );

  LOGLN( 1, "\n" );
  return internal;
}

void RwObjectService::write(std::vector<DrawableObject*> objects_list, std::string file_path)
{
  std::ofstream myfile;
  myfile.open(file_path);
  myfile << "# Starting the file objects\n\n";

  for (auto it_obj = objects_list.begin(); it_obj != objects_list.end(); ++it_obj)
  {
    myfile << "o " + (*it_obj)->getName() + "\n\n";
    std::vector<Coordinate*> objectCoordinates = (*it_obj)->worldCoordinates();

    for (std::vector<Coordinate*>::iterator it_cord = objectCoordinates.begin(); it_cord != objectCoordinates.end(); ++it_cord)
    {
      std::string line_cord = "v " + std::to_string((*it_cord)->x) + " " + std::to_string((*it_cord)->y)
          + " " + std::to_string((*it_cord)->z) + "\n";

      myfile << line_cord;
    }

    myfile << "\nf ";

    for (unsigned int i = 1; i < objectCoordinates.size()+1; i++)
    {
      myfile << std::to_string(i) + " ";
    }

    myfile << "\n\n";
  }

  myfile.close();
}
