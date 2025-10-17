#ifndef __WANJPLAYER__HPP
#define __WANJPLAYER__HPP

#include "player_ui.hpp"
#include "widgets.hpp"
#include <cstddef>
#include <memory>
#include <wx/wx.h>

class wxSimplebook;

namespace gui {
class StatusBar; // Forward declaration
class Sidebar;   // Forward declaration
}

namespace gui::player {
class Playlist; // Forward declaration to avoid circular reference
}

class WanjPlayer : public wxApp
{
public:
  bool OnInit() override;
  
private:
};

class PlayerFrame : public wxFrame
{
public:
  PlayerFrame();
  ~PlayerFrame();

private: // UI Components
  wxPanel* video_canvas_pane;
  wxMediaCtrl* media_ctrl;
  wxSimplebook* media_book;

  gui::player::MediaControls* player_ctrls;
  gui::player::Playlist* playlist;
  gui::StatusBar* status_bar;
  gui::Sidebar* sidebar;
  gui::PlayerCanvas* player_canvas;
  wxSplitterWindow* splitter;
  wxPanel* playlist_pane;
  bool playlist_visible;

  std::size_t current_index;

private: // Helper methods
  void BindMenuEvents();
  void BindMediaEvents();

private: // Events
  // UI Events
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnFileOpen(wxCommandEvent& event);
  void OnFilesOpen(wxCommandEvent& event);
  void OnLicense(wxCommandEvent& event);
  void OnPreferences(wxCommandEvent& event);

  // Media Events
  void OnMediaLoaded(wxMediaEvent& event);
  void OnMediaPlay(wxMediaEvent& event);
  void OnMediaPause(wxMediaEvent& event);
  void OnMediaStop(wxMediaEvent& event);
  void OnMediaFinished(wxMediaEvent& event);
  
  // Playlist toggle functionality
  void OnTogglePlaylist(wxCommandEvent& event);
};

enum
{
  ID_OPENFILE = wxID_HIGHEST + 1,
  ID_OPEN_FILES,
  ID_PREFS,
  ID_LICENSE,
  ID_MEDIA_LOADED,
  ID_MEDIA_STOP,
  ID_MEDIA_PLAY,
  ID_MEDIA_PAUSE,
  ID_MEDIA_FINISHED,
  ID_MEDIA_CANVAS,
  ID_MEDIA_CTRL,
  ID_TOGGLE_PLAYLIST
};

#endif // !__WANJPLAYER__HPP