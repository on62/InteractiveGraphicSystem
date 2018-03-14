#ifndef GTKMM_APP_ADD_TRANSFORMATION_WINDOW
#define GTKMM_APP_ADD_TRANSFORMATION_WINDOW

#include <string>
#include <iostream>

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/listviewtext.h>

#include "point.h"
#include "line.h"
#include "polygon.h"
#include "viewport.h"
#include "coordinate.h"

class AddTransformationWindow
{
public:
  AddTransformationWindow(ViewPort* viewPort);
  ~AddTransformationWindow();
  Gtk::Window* getWindow();

private:
  ViewPort* viewPort;
  Gtk::Window window;

  Gtk::Box       m_vbox;
  Gtk::Box       m_hbox;
  Gtk::Notebook  m_notebook;

  std::string            new_object_name;
  std::list<Coordinate*> polygon_cord_list;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::ListViewText m_ListViewText;

  Gtk::Grid rotation_grid;
  Gtk::Grid translation_grid;
  Gtk::Grid scaling_grid;

  Gtk::Button button_close;
  Gtk::Button button_save_transformation;

  Gtk::Entry main_value_field;

  void on_button_close();
  void on_button_save_transformation();

  void create_action_tabs();
  void create_scrolling_items_list();
};
#endif //GTKMM_APP_ADD_TRANSFORMATION_WINDOW