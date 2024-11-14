#include "menubar.hpp"

gui::Menubar::Menubar(wxFrame* parent)
  : parent_(parent) {};

void
gui::Menubar::create_menubar()
{
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_Hello,
                   "&Hello...\tCtrl-H",
                   "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu* menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  parent_->SetMenuBar(menuBar);
}
