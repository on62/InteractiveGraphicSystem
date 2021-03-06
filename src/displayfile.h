#ifndef GTKMM_APP_DISPLAY_FILE
#define GTKMM_APP_DISPLAY_FILE

#include <map>
#include <list>
#include <string>

#include "drawableobject.h"
class World;

/**
 * `Random Scan Display` motivou o DisplayFile: Lista de todos os objetos constantemente
 * redesenhados na tela.
 */
template<typename Type>
class DisplayFile
{
public:
  DisplayFile() {
  }

  virtual ~DisplayFile() {
  }

  void addObject(Type object) {
    this->objectsMap[object->getName()] = object;
  }

  void removeObject(Type object) {
    this->removeObjectByName(object->getName());
  }

  void removeObjectByName(std::string name) {
    this->objectsMap.erase(name);
  }

  void apply(Transformation &transformation)
  {
    for( auto iterator = objectsMap.begin(); iterator != objectsMap.end(); iterator++ ) {
      iterator->second->apply(transformation);
    }
  }

  void apply(const Transformation& transformation, const Axes& axes)
  {
    for( auto iterator = objectsMap.begin(); iterator != objectsMap.end(); iterator++ ) {
      iterator->second->updateWindowCoordinates(transformation);
      iterator->second->updateClippingCoordinates(axes);
    }
  }

  Type apply(std::string object_name, Transformation &transformation)
  {
    auto object = this->getObjectByName(object_name);
    object->apply(transformation);
    return object;
  }

  bool isObjectOnByName(std::string name) {
    auto iterator = this->objectsMap.find(name);

    if( iterator == this->objectsMap.end() ) {
      return false;
    }

    return true;
  }

  Type getObjectByName(std::string name)
  {
    LOG(8, "Entering: %s", name);
    auto iterator = this->objectsMap.find(name);

    if( iterator == this->objectsMap.end() )
    {
      auto error = tfm::format("There are not objects within the name: `%s`", name);

      LOG( 1, "%s", error );
      throw std::runtime_error(error);
    }

    LOG(8, "Object: %s", *iterator->second);
    return iterator->second;
  }

  std::vector<std::string> objectNames() const {
    std::vector<std::string> list;

    for(auto iterator = objectsMap.begin(); iterator != objectsMap.end(); iterator++) {
      list.push_back(iterator->first);
    }

    return list;
  }

  std::vector<Type> getObjects() const {
    std::vector<Type> list;

    for(auto iterator = objectsMap.begin(); iterator != objectsMap.end(); iterator++) {
      list.push_back(iterator->second);
    }

    return list;
  }

protected:
  std::map<std::string, Type> objectsMap;
};

#endif // GTKMM_APP_DISPLAY_FILE
