// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"
#include "player_ui.hpp"
#include "widgets.hpp"
#include <algorithm>
#include <memory>

wxIMPLEMENT_APP(WanjPlayer);

bool
WanjPlayer::OnInit()
{
  wxInitAllImageHandlers();
  PlayerFrame* frame = new PlayerFrame();
  frame->Show(true);
  return true;
}

PlayerFrame::PlayerFrame()
  : wxFrame(nullptr,
            wxID_ANY,
            "WanjPlayer",
            wxPoint(100, 80),
            wxSize(1000, 800))

{
  // SET APP Icon
  wxIcon icon;
  icon.LoadFile("../assets/logo/wanjplayer-64x64.png", wxBITMAP_TYPE_PNG);
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
  video_canvas_pane = new wxPanel(splitter, wxID_ANY);

  // Set images
  default_image = new wxImage();
  audio_banner = new wxImage();
  default_image->LoadFile("../assets/logo/wanjplayer-256x256.png",
                          wxBITMAP_TYPE_PNG);
  audio_banner->LoadFile("../assets/logo/wanjplayer-64x64.png",
                         wxBITMAP_TYPE_PNG);

  canvas_banner = new wxStaticBitmap(
    video_canvas_pane, wxID_ANY, wxBitmapBundle::FromImage(*default_image));

  wxBoxSizer* video_sizer = new wxBoxSizer(wxVERTICAL);
  video_sizer->Add(canvas_banner, 1, wxEXPAND);

  video_canvas_pane->SetSizer(video_sizer);

  media_ctrl = new wxMediaCtrl(video_canvas_pane,
                               wxID_ANY,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxMC_NO_AUTORESIZE,
                               wxMEDIABACKEND_GSTREAMER);

  video_sizer->Add(media_ctrl, 1, wxEXPAND);

  // Initially hide the media control
  media_ctrl->Hide();

  // Left Pane for the midea queue
  wxPanel* media_queue_pane = new wxPanel(splitter, wxID_ANY);

  // Set the splitter to split intwo two vertical panes
  splitter->SplitVertically(media_queue_pane, video_canvas_pane, 250);
  splitter->SetMinimumPaneSize(100);
  // splitter->Unsplit();

  /**
   *
   * CREATE THE STATUS BAR
   *
   */
  gui::StatusBar* player_statusbar = new gui::StatusBar(this);
  player_statusbar->create_statusbar(
    wxString::FromCDouble(media_ctrl->GetVolume() * 100, -1),
    wxString::FromCDouble(media_ctrl->GetPlaybackRate()),
    wxString::Format(_T("%lld"), media_ctrl->Length() / 60000));

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
  Bind(wxEVT_MENU, &PlayerFrame::OnPreferences, this, ID_PREFS);
  Bind(wxEVT_MENU, &PlayerFrame::OnLicense, this, ID_LICENSE);
  Bind(wxEVT_MENU, &PlayerFrame::OnAbout, this, wxID_ABOUT);

  // BIND MEDIA EVENTS
  Bind(wxEVT_MEDIA_LOADED, &PlayerFrame::OnMediaLoaded, this, ID_MEDIA_LOADED);
  Bind(wxEVT_MEDIA_PLAY, &PlayerFrame::OnMediaPlay, this, ID_MEDIA_PLAY);
  Bind(wxEVT_MEDIA_PAUSE, &PlayerFrame::OnMediaPause, this, ID_MEDIA_PAUSE);
  Bind(wxEVT_MEDIA_STOP, &PlayerFrame::OnMediaStop, this, ID_MEDIA_STOP);
  Bind(
    wxEVT_MEDIA_FINISHED, &PlayerFrame::OnMediaUnload, this, ID_MEDIA_FINISHED);
};
