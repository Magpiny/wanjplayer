// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"
#include "player_ui.hpp"
#include "statusbar.hpp"
#include "widgets.hpp"
#include "sidebar.hpp"
#include "utils.hpp"
#include <algorithm>
#include <memory>

wxIMPLEMENT_APP(WanjPlayer);

bool
WanjPlayer::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  // Set essential environment variables for video compatibility
  wxSetEnv("GDK_BACKEND", "x11");
  wxSetEnv("GST_GL_DISABLED", "1");
  wxSetEnv("LIBGL_ALWAYS_SOFTWARE", "1");
  wxSetEnv("GST_VIDEO_SINK", "ximagesink");
  wxSetEnv("GST_PLUGIN_FEATURE_DISABLE", "glimagesink,glsinkbin,gtkglsink");

  utils::LogUtils::LogInfo("WanjPlayer starting up");
  
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
  , playlist_visible(true)
  , playlist_pane(nullptr)
  , sidebar(nullptr)
{
  utils::LogUtils::LogInfo("Initializing PlayerFrame");
  
  // SET APP Icon
  wxIcon icon;
  if (!icon.LoadFile("assets/logo/wanjplayer-64x64.png",
                     wxBITMAP_TYPE_PNG)) {
    utils::LogUtils::LogError("Failed to load application icon");
  } else {
    SetIcon(icon);
    utils::LogUtils::LogInfo("Application icon loaded successfully");
  }

  /**
   * CREATE THE MENU BAR
   */
  gui::Menubar* player_menubar = new gui::Menubar(this);
  player_menubar->create_menubar();

  /**
   * CREATE THE SIDEBAR AND MAIN LAYOUT
   */
  sidebar = new gui::Sidebar(this);
  if (!sidebar) {
    utils::LogUtils::LogError("Failed to create sidebar");
    return;
  }
  
  // Create the main layout using the sidebar
  sidebar->CreateMainLayout();
  
  // Get references to components created by sidebar
  splitter = sidebar->GetMainSplitter();
  playlist_pane = sidebar->GetPlaylistPane();
  video_canvas_pane = sidebar->GetVideoPane();
  playlist = sidebar->GetPlaylist();
  player_ctrls = sidebar->GetMediaControls();
  media_ctrl = sidebar->GetMediaCtrl();

  /**
   * CREATE THE STATUS BAR
   */
  status_bar = new gui::StatusBar(this);
  status_bar->create_statusbar();
  
  // Connect status bar to sidebar components
  sidebar->SetStatusBar(status_bar);
  
  // Connect all components
  sidebar->ConnectComponents();
  
  // Show video backend status in status bar
  status_bar->set_system_message("Ready - Video mode: Wayland-safe");

  /**
   * BIND GUI EVENTS HERE
   */
  BindMenuEvents();
  BindMediaEvents();
  
  // Setup accessibility
  sidebar->SetupAccessibility();
  
  utils::LogUtils::LogInfo("PlayerFrame initialization complete");
}

void PlayerFrame::BindMenuEvents()
{
  Bind(wxEVT_MENU, &PlayerFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &PlayerFrame::OnFileOpen, this, ID_OPENFILE);
  Bind(wxEVT_MENU, &PlayerFrame::OnFilesOpen, this, ID_OPEN_FILES);
  Bind(wxEVT_MENU, &PlayerFrame::OnPreferences, this, ID_PREFS);
  Bind(wxEVT_MENU, &PlayerFrame::OnLicense, this, ID_LICENSE);
  Bind(wxEVT_MENU, &PlayerFrame::OnAbout, this, wxID_ABOUT);
  
  // Playlist toggle is now handled by sidebar
  Bind(wxEVT_MENU, &PlayerFrame::OnTogglePlaylist, this, ID_TOGGLE_PLAYLIST);
}

void PlayerFrame::BindMediaEvents()
{
  Bind(wxEVT_MEDIA_LOADED, &PlayerFrame::OnMediaLoaded, this);
  Bind(wxEVT_MEDIA_PLAY, &PlayerFrame::OnMediaPlay, this);
  Bind(wxEVT_MEDIA_PAUSE, &PlayerFrame::OnMediaPause, this);
  Bind(wxEVT_MEDIA_STOP, &PlayerFrame::OnMediaStop, this);
  Bind(wxEVT_MEDIA_FINISHED, &PlayerFrame::OnMediaFinished, this);
}

void PlayerFrame::OnTogglePlaylist(wxCommandEvent& event)
{
  if (sidebar) {
    sidebar->TogglePlaylistVisibility();
  }
  event.Skip();
}

PlayerFrame::~PlayerFrame()
{
  utils::LogUtils::LogInfo("PlayerFrame shutting down");
  
  // Components are automatically cleaned up by wxWidgets
  // The sidebar will save its layout settings in its destructor
}