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
            wxPoint(200, 200),
            wxSize(1000, 800))

{
  // SET APP Icon
  wxIcon icon;
  if (!icon.LoadFile(wxString::FromUTF8("../assets/logo/wanjplayer-64x64.png"),
                     wxBITMAP_TYPE_PNG)) {
    wxLogError("Failed to load Icon");
  };
  SetIcon(icon);

  // prop -> Stands for properties; properties of mediacontrols window i.e pane
  // that contains play and pause buttons
  wxAuiPaneInfo prop_mediactrls;
  prop_mediactrls.BottomDockable();
  prop_mediactrls.Caption("wanjplayer");
  prop_mediactrls.Floatable();
  prop_mediactrls.FloatingSize(wxSize(400, 80));
  prop_mediactrls.Dockable();
  prop_mediactrls.Layer(4);

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

  media_ctrl = new wxMediaCtrl(video_canvas_pane,
                               wxID_ANY,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxMC_NO_AUTORESIZE,
                               wxMEDIABACKEND_GSTREAMER);

  player_ctrls = new gui::player::MediaControls(video_canvas_pane, media_ctrl);

  //  video_sizer->Add(canvas_banner, 1, wxEXPAND);
  video_sizer->Add(media_ctrl, 9.4, wxEXPAND);
  video_sizer->Add(player_ctrls, 0.6, wxEXPAND);

  video_canvas_pane->SetSizer(video_sizer);

  // Left Pane for the media queue
  wxPanel* media_queue_pane = new wxPanel(splitter, wxID_ANY);
  playlist = new gui::Playlist(media_queue_pane, wxID_ANY);

  wxBoxSizer* md_queue_sizer = new wxBoxSizer(wxVERTICAL);
  md_queue_sizer->Add(playlist, 1, wxEXPAND);
  media_queue_pane->SetSizerAndFit(md_queue_sizer);

  // Set the splitter to split intwo two vertical panes
  splitter->SplitVertically(media_queue_pane, video_canvas_pane, 250);
  splitter->SetMinimumPaneSize(100);

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
  Bind(wxEVT_MENU, &PlayerFrame::OnFilesOpen, this, ID_OPEN_FILES);
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
