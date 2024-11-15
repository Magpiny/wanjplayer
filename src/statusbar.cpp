#include "statusbar.hpp"

gui::StatusBar::StatusBar(wxFrame* parent)
  : _parent(parent) {};

void
gui::StatusBar::create_statusbar()
{
  _parent->CreateStatusBar(4);
  _parent->SetStatusText(wxT("Welcome to wxWidgets!"), 1);
};
