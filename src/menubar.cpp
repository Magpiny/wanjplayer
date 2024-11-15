#include "menubar.hpp"

gui::Menubar::Menubar(wxFrame* parent)
  : _parent(parent) {};

void
gui::Menubar::create_menubar()
{
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_OPENFILE, "&Open File\tCtrl-O");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu* menuHelp = new wxMenu;
  menuHelp->Append(ID_PREFS, "&Preferences");
  menuHelp->Append(wxID_ABOUT);
  menuHelp->Append(ID_LICENSE, "&Licence");

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  _parent->SetMenuBar(menuBar);
}
