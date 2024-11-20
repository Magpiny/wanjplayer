#include "menubar.hpp"

gui::Menubar::Menubar(wxFrame* parent)
  : _parent(parent) {};

void
gui::Menubar::create_menubar()
{
  wxMenu* menu_file = new wxMenu;
  menu_file->Append(ID_OPENFILE, "&Open File\tCtrl-O");
  menu_file->Append(ID_MEDIA_LOADED, "&Play");
  menu_file->AppendSeparator();
  menu_file->Append(wxID_EXIT);

  wxMenu* menu_help = new wxMenu;
  menu_help->Append(ID_PREFS, "&Preferences");
  menu_help->Append(wxID_ABOUT);
  menu_help->Append(ID_LICENSE, "&Licence");

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menu_file, "&File");
  menuBar->Append(menu_help, "&Help");

  _parent->SetMenuBar(menuBar);
}
