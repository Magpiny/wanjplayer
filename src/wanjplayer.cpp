// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"
#include "player_ui.hpp"
#include "wx_widgets.hpp"
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
  if (!icon.LoadFile("./assets/logo/wanjplayer-64x64.png", wxBITMAP_TYPE_PNG))
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
  wxMediaCtrl* media = new wxMediaCtrl(video_canvas_pane,
                                       wxID_ANY,
                                       wxEmptyString,
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       wxMC_NO_AUTORESIZE,
                                       wxMEDIABACKEND_GSTREAMER);

  if (media->Load("../assets/sample-media/Oti.mp4")) {
    media->ShowPlayerControls();
    media->Play();
  } else {
    wxMessageBox(
      "Failed to load media file", "PlayBackError", wxOK | wxICON_ERROR);
  }
  wxBoxSizer* video_sizer = new wxBoxSizer(wxVERTICAL);
  video_sizer->Add(media, 1, wxEXPAND);
  video_canvas_pane->SetSizer(video_sizer);

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
  gui::StatusBar* player_statusbar = new gui::StatusBar(this);
  player_statusbar->create_statusbar();

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
  wxMessageBox(
    "File Open dialog box", "From wanjare", wxOK | wxICON_INFORMATION);
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
