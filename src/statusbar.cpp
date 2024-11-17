#include "statusbar.hpp"

gui::StatusBar::StatusBar(wxFrame* parent)
  : _parent(parent) {};

void
gui::StatusBar::create_statusbar(wxString mssg1, wxString mssg2, wxString mssg3)
{
  _parent->CreateStatusBar(4);
  _parent->SetStatusText(mssg1, 1);
  _parent->SetStatusText(mssg2, 2);
  _parent->SetStatusText(mssg3, 3);
};
