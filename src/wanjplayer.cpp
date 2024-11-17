// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"
#include "player_ui.hpp"
#include "wx_widgets.hpp"
#include <algorithm>
#include <memory>

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
  // SET APP Icon
  wxIcon icon;
  if (!icon.LoadFile("../assets/logo/wanjplayer-64x64.png", wxBITMAP_TYPE_PNG))
    wxMessageBox(
      "Failed to load Icon", "Icon Failure", wxOK | wxICON_INFORMATION);
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
  wxPanel* video_canvas_pane = new wxPanel(splitter, wxID_ANY);
  media_ctrl = new wxMediaCtrl(video_canvas_pane,
                               wxID_ANY,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxMC_NO_AUTORESIZE,
                               wxMEDIABACKEND_GSTREAMER);

  wxBoxSizer* video_sizer = new wxBoxSizer(wxVERTICAL);
  video_sizer->Add(media_ctrl, 1, wxEXPAND);
  video_canvas_pane->SetSizer(video_sizer);

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
  Bind(wxEVT_MENU, &PlayerFrame::OnMediaLoaded, this, ID_MEDIA_LOADED);
};

void
PlayerFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
};

void
PlayerFrame::OnAbout(wxCommandEvent& event)
{
  std::unique_ptr<gui::About> about = std::make_unique<gui::About>();
  about->create_about_page();
};

void
PlayerFrame::OnFileOpen(wxCommandEvent& event)
{
  wxFileDialog open_file_dialog(
    this,
    _("Open Media File"),
    "",
    "",
    "Media files "
    "(*.mp3;*.mp4;*.mkv;*.avi;*.wav;*.aac;*.m4a;*.flac;*.ogg)|*.avi;*.mp3;*."
    "mp4;*.mkv;*.wav;*.aac;*.m4a;*.flac;*.ogg",
    wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (open_file_dialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  // set the file path dir
  wxString file_path = open_file_dialog.GetPath();
  if (media_ctrl && media_ctrl->Load(file_path)) {
    media_ctrl->Play();
    media_ctrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_VOLUME);
  } else {
    wxLogError("Failed to load media");
  }
};

void
PlayerFrame::OnPreferences(wxCommandEvent& event)
{
  wxMessageBox(
    "Opens App Preferences", "App preferences", wxOK | wxICON_INFORMATION);
};

void
PlayerFrame::OnLicense(wxCommandEvent& event)
{
  gui::LicenseDialog license_dialog =
    gui::LicenseDialog(this, "License Agreement");
  license_dialog.load_license("../assets/LICENSE");
  license_dialog.DoLayoutAdaptation();
  license_dialog.ShowModal();
};

void
PlayerFrame::OnMediaLoaded(wxCommandEvent& event)
{
  /* std::unique_ptr<wxMediaCtrl> media =*/
  /*std::make_unique<wxMediaCtrl>(this,*/
  /*ID_MEDIA_LOADED,*/
  /*wxEmptyString,*/
  /*wxDefaultPosition,*/
  /*wxDefaultSize,*/
  /*wxMC_NO_AUTORESIZE,*/
  /*wxMEDIABACKEND_GSTREAMER);*/
  /*media->Load("../assets/sample-media/FORM1Choir.mp4");*/
  /*media->Play();*/
  /*wxMessageBox(media->GetState, "PlayBackStatu", wxOK | wxERROR_ICON);*/
}
