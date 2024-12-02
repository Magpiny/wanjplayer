// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"

wxIMPLEMENT_APP(WanjPlayer);

bool
WanjPlayer::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  wxInitAllImageHandlers();
  PlayerFrame* frame = new PlayerFrame();
  frame->Show(true);
  return true;
}

PlayerFrame::PlayerFrame()
  : wxFrame(nullptr,
            wxID_ANY,
            "WanjPlayer",
            wxPoint(200, 150),
            wxSize(950, 700))

{
  // SET APP Icon
  wxIcon icon;
  if (!icon.LoadFile("../assets/logo/wanjplayer-32x32.png",
                     wxBITMAP_TYPE_PNG)) {
    wxLogError("Failed to load Icon");
  };
  icon.LoadFile("../assets/logo/wanjplayer-32x32.png", wxBITMAP_TYPE_PNG);
  SetIcon(icon);

  /**
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
  video_canvas_pane = new wxPanel(splitter, ID_MEDIA_CANVAS);

  wxBoxSizer* video_sizer = new wxBoxSizer(wxVERTICAL);

  media_ctrl = new wxMediaCtrl(video_canvas_pane,
                               ID_MEDIA_CTRL,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxMC_NO_AUTORESIZE,
                               wxMEDIABACKEND_GSTREAMER);

  player_ctrls = new gui::player::MediaControls(video_canvas_pane, media_ctrl);

  video_sizer->Add(media_ctrl, 9.4, wxEXPAND);
  video_sizer->Add(player_ctrls, 0.6, wxEXPAND);

  video_canvas_pane->SetSizer(video_sizer);

  // Left Pane for the media queue
  wxPanel* media_queue_pane = new wxPanel(splitter, wxID_ANY);
  playlist = new gui::player::Playlist(media_queue_pane, wxID_ANY);

  wxStaticBoxSizer* sidebar_bottom =
    new wxStaticBoxSizer(wxHORIZONTAL, media_queue_pane, "wanjplayer");

  wxButton* btn_clear = new wxButton(media_queue_pane, wxID_CLEAR);

  wxStaticText* num_of_loaded_files =
    new wxStaticText(media_queue_pane,
                     wxID_ANY,
                     "3 files loaded",
                     wxDefaultPosition,
                     wxDefaultSize,
                     wxALIGN_CENTRE_HORIZONTAL);
  num_of_loaded_files->SetForegroundColour(wxColour(23, 109, 210));

  sidebar_bottom->Add(num_of_loaded_files, 2, wxEXPAND);
  sidebar_bottom->Add(btn_clear, 1, wxEXPAND | wxALL);

  wxBoxSizer* md_queue_sizer = new wxBoxSizer(wxVERTICAL);
  md_queue_sizer->Add(playlist, 9, wxEXPAND);
  md_queue_sizer->Add(sidebar_bottom, 1, wxEXPAND);
  media_queue_pane->SetSizerAndFit(md_queue_sizer);

  // Set the splitter to split intwo two vertical panes
  splitter->SplitVertically(media_queue_pane, video_canvas_pane, 200);
  splitter->SetMinimumPaneSize(5);

  /**
   *
   * CREATE THE STATUS BAR
   *
   */

  player_statusbar = new gui::StatusBar(this);

  wxLogStatus(wxString::Format("%lld", media_ctrl->Length()));

  // Layout the splitter on the frame
  wxBoxSizer* player_window_sizer = new wxBoxSizer(wxVERTICAL);
  player_window_sizer->Add(splitter, 1, wxEXPAND);
  this->SetSizer(player_window_sizer);
  this->Layout();

  /**
   * BIND GUI EVENTS HERE
   *
   */
  Bind(wxEVT_MENU, &PlayerFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &PlayerFrame::OnFileOpen, this, ID_OPENFILE);
  Bind(wxEVT_MENU, &PlayerFrame::OnFilesOpen, this, ID_OPEN_FILES);
  Bind(wxEVT_MENU, &PlayerFrame::OnPreferences, this, ID_PREFS);
  Bind(wxEVT_MENU, &PlayerFrame::OnLicense, this, ID_LICENSE);
  Bind(wxEVT_MENU, &PlayerFrame::OnAbout, this, wxID_ABOUT);

  // BIND MEDIA EVENTS
  Bind(wxEVT_MEDIA_LOADED, &PlayerFrame::OnMediaLoaded, this, ID_MEDIA_LOADED);
  Bind(wxEVT_MEDIA_PLAY, &PlayerFrame::OnMediaPlay, this, ID_MEDIA_PLAY);
  Bind(wxEVT_MEDIA_PAUSE, &PlayerFrame::OnMediaPause, this, ID_MEDIA_PAUSE);
  Bind(wxEVT_MEDIA_STOP, &PlayerFrame::OnMediaStop, this, ID_MEDIA_STOP);
  Bind(wxEVT_MEDIA_FINISHED,
       &PlayerFrame::OnMediaFinished,
       this,
       ID_MEDIA_FINISHED);
};
