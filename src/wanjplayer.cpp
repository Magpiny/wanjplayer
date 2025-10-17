// Start of wxWidgets "Hello World" Program
#include "wanjplayer.hpp"
#include "player_ui.hpp"
#include "statusbar.hpp"
#include "widgets.hpp"
#include "main_layout.hpp"
#include "utils.hpp"
#include <algorithm>
#include <memory>
#include <wx/dir.h>
#include <wx/iconbndl.h>
#include <wx/config.h>

wxIMPLEMENT_APP(WanjPlayer);

bool
WanjPlayer::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  // Initialize config
  wxConfigBase::Set(new wxConfig("WanjPlayer", wxEmptyString, wxEmptyString, wxEmptyString, wxCONFIG_USE_SUBDIR));
  wxConfigBase* config = wxConfigBase::Get();

  // Apply Logging settings
  config->SetPath("/Logging");
  utils::LogUtils::SetLogLevel(static_cast<utils::LogUtils::LogLevel>(config->Read("LogLevel", (long)utils::LogUtils::LogLevel::INFO)));
  utils::LogUtils::EnableFileOutput(config->Read("FileLoggingEnabled", false), config->Read("LogFilePath", utils::LogUtils::GetDefaultLogFileName()));
  utils::LogUtils::SetMaxLogFileSize(config->Read("MaxLogSizeMB", 10L) * 1024 * 1024);
  utils::LogUtils::SetMaxLogFiles(config->Read("MaxLogFiles", 5L));

  // Apply Performance settings
  config->SetPath("/Performance");
  utils::PerformanceUtils::EnableProfiling(config->Read("ProfilingEnabled", true));
  utils::PerformanceUtils::SetMaxCacheSize(config->Read("MaxCacheSizeMB", 100L) * 1024 * 1024);

  // Set essential environment variables for video compatibility
  wxSetEnv("GDK_BACKEND", "x11");
  wxSetEnv("GST_GL_DISABLED", "1");
  wxSetEnv("LIBGL_ALWAYS_SOFTWARE", "1");
  wxSetEnv("GST_VIDEO_SINK", "ximagesink");
  wxSetEnv("GST_PLUGIN_FEATURE_DISABLE", "glimagesink,glsinkbin,gtkglsink");
  wxSetEnv("GST_DEBUG", "3");

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
  , main_layout(nullptr)
  , player_ui_control(nullptr)
{
  utils::LogUtils::LogInfo("Initializing PlayerFrame");
  
  // SET APP Icon
  wxIconBundle icons;
  wxString icon_dir = "assets/logo/";

  if (!wxDirExists(icon_dir)) {
    icon_dir = "../assets/logo/";
  }

  if (wxDirExists(icon_dir)) {
    icons.AddIcon(icon_dir + "wanjplayer-16x16.png", wxBITMAP_TYPE_PNG);
    icons.AddIcon(icon_dir + "wanjplayer-32x32.png", wxBITMAP_TYPE_PNG);
    icons.AddIcon(icon_dir + "wanjplayer-64x64.png", wxBITMAP_TYPE_PNG);
    icons.AddIcon(icon_dir + "wanjplayer-256x256.png", wxBITMAP_TYPE_PNG);
  }

  if (!icons.IsEmpty()) {
    SetIcons(icons);
    utils::LogUtils::LogInfo("Application icons loaded successfully");
  } else {
    utils::LogUtils::LogError("Failed to load application icons. Icon directory not found.");
  }

  /**
   * CREATE THE MENU BAR
   */
  utils::LogUtils::LogInfo("Creating Menubar");
  gui::Menubar* player_menubar = new gui::Menubar(this);
  player_menubar->create_menubar();
  utils::LogUtils::LogInfo("Menubar created");

  /**
   * CREATE THE MAIN LAYOUT
   */
  utils::LogUtils::LogInfo("Creating MainLayout");
  main_layout = new gui::MainLayout(this);
  if (!main_layout) {
    utils::LogUtils::LogError("Failed to create main layout");
    return;
  }
  utils::LogUtils::LogInfo("MainLayout created");
  
  // Create the main layout using the main_layout
  utils::LogUtils::LogInfo("Creating main layout");
  main_layout->CreateMainLayout();
  utils::LogUtils::LogInfo("Main layout created");
  
  // Get references to components created by main_layout
  utils::LogUtils::LogInfo("Getting references to components");
  splitter = main_layout->GetMainSplitter();
  playlist_pane = main_layout->GetPlaylistPane();
  video_canvas_pane = main_layout->GetVideoPane();
  playlist = main_layout->GetPlaylist();
  player_ctrls = main_layout->GetMediaControls();
  player_ui_control = main_layout->GetPlayerUIControl();
  utils::LogUtils::LogInfo("References to components obtained");

  /**
   * CREATE THE STATUS BAR
   */
  utils::LogUtils::LogInfo("Creating StatusBar");
  status_bar = new gui::StatusBar(this);
  status_bar->create_statusbar();
  utils::LogUtils::LogInfo("StatusBar created");
  
  // Connect status bar to main_layout components
  main_layout->SetStatusBar(status_bar);
  
  // Connect all components
  main_layout->ConnectComponents();
  
  // Show video backend status in status bar
  status_bar->set_system_message("Ready - Video mode: Wayland-safe");

  /**
   * BIND GUI EVENTS HERE
   */
  BindMenuEvents();
  BindMediaEvents();
  
  // Setup accessibility
  main_layout->SetupAccessibility();

  // Restore window geometry and transparency
  wxConfigBase* config = wxConfig::Get();
  config->SetPath("/General");
  if (config->Read("RememberGeometry", true)) {
      utils::GuiUtils::RestoreWindowGeometry(this, "PlayerFrame");
  }
  SetTransparent(config->Read("Transparency", 255L));
  
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
  
  // Playlist toggle is now handled by main_layout
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
  if (main_layout) {
    main_layout->TogglePlaylistVisibility();
  }
  event.Skip();
}

PlayerFrame::~PlayerFrame()
{
  // Save window geometry
  wxConfigBase* config = wxConfig::Get();
  config->SetPath("/General");
  if (config->Read("RememberGeometry", true)) {
      utils::GuiUtils::SaveWindowGeometry(this, "PlayerFrame");
  }

  utils::LogUtils::LogInfo("PlayerFrame shutting down");
  
  // Components are automatically cleaned up by wxWidgets
  // The main_layout will save its layout settings in its destructor
}
