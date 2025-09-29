#include "statusbar.hpp"
#include "utils.hpp"
#include <wx/filename.h>

gui::StatusBar::StatusBar(wxFrame* parent)
  : _parent(parent)
{
}

void
gui::StatusBar::create_statusbar()
{
  _parent->CreateStatusBar(4);
  
  // Set status bar field widths
  // Field 0: System messages (flexible width)
  // Field 1: Filename (fixed width)
  // Field 2: Duration (fixed width)
  // Field 3: Playback info (fixed width)
  int widths[] = {-1, 300, 150, 200};
  _parent->GetStatusBar()->SetStatusWidths(4, widths);
  
  // Set initial messages
  _parent->SetStatusText("Ready", 0);
  _parent->SetStatusText("No file loaded", 1);
  _parent->SetStatusText("0s", 2);
  _parent->SetStatusText("Stopped", 3);
};

void
gui::StatusBar::update_file_info(const wxString& filename, wxFileOffset duration_ms)
{
  if (!_parent || !_parent->GetStatusBar()) {
    return;
  }
  
  // Extract just the filename without extension from full path
  wxFileName file(filename);
  wxString display_name = file.GetName(); // GetName() returns filename without extension
  
  // Limit filename display length
  if (display_name.length() > 40) {
    display_name = display_name.Left(37) + "...";
  }
  
  // Update filename field
  _parent->SetStatusText(display_name, 1);
  
  // Update duration field - set initial format as 0s / total_duration
  if (duration_ms > 0) {
    wxString duration_display = "0s / " + utils::TimeFormatter::FormatTime(duration_ms);
    _parent->SetStatusText(duration_display, 2);
  } else {
    _parent->SetStatusText("0s", 2);
  }
  

};

void
gui::StatusBar::update_playback_info(const wxString& status)
{
  if (!_parent || !_parent->GetStatusBar()) {
    return;
  }
  
  _parent->SetStatusText(status, 3);
};

void
gui::StatusBar::set_system_message(const wxString& message)
{
  if (!_parent || !_parent->GetStatusBar()) {
    return;
  }
  
  _parent->SetStatusText(message, 0);
};



void
gui::StatusBar::set_duration_display(const wxString& duration_str)
{
  if (!_parent || !_parent->GetStatusBar()) {
    return;
  }
  
  _parent->SetStatusText(duration_str, 2);
}


