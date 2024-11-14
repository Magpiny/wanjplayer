// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"
#include "menubar.hpp"
#include "statusbar.hpp"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>

wxIMPLEMENT_APP(WanjPlayer);

bool
WanjPlayer::OnInit()
{
  PlayerFrame* frame = new PlayerFrame();
  frame->Show(true);
  return true;
}

PlayerFrame::PlayerFrame()
  : wxFrame(nullptr, wxID_ANY, "WanjPlayer", wxPoint(100, 80), wxSize(800, 500))

{
  /**
   *
   * CREATE THE MENU BAR
   *
   */
  gui::Menubar* player_menubar = new gui::Menubar(this);
  player_menubar->create_menubar();

  /**
   *
   * DESIGN THE MAIN APP WINDOW
   *
   */
  // Set the splitter
  wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);

  // Right Window for the video canvas
  wxPanel* video_canvas_pane = new wxPanel(splitter, wxID_ANY);

  // Left Pane for the midea queue
  wxPanel* media_queue_pane = new wxPanel(splitter, wxID_ANY);

  // Set the splitter to split intwo two vertical panes
  splitter->SplitVertically(media_queue_pane, video_canvas_pane, 250);
  splitter->SetMinimumPaneSize(100);
  // splitter->Unsplit();

  // Layout the splitter on the frame
  wxBoxSizer* player_window_sizer = new wxBoxSizer(wxVERTICAL);
  player_window_sizer->Add(splitter, 1, wxEXPAND);
  this->SetSizer(player_window_sizer);
  this->Layout();

  /**
   *
   * CREATE THE STATUS BAR
   *
   */
}
