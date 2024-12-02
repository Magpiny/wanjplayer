#ifndef __WANJPLAYER__HPP
#define __WANJPLAYER__HPP

#include "player_ui.hpp"
#include "widgets.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>

#include "w_timer.hpp"
#include <wx/wx.h>

namespace gui::player {
class Playlist; // Forward declaration to avoid circular reference
}
namespace gui {
class StatusBar;
}

class WanjPlayer : public wxApp
{
public:
  bool OnInit() override;
};

class PlayerFrame : public wxFrame
{
public:
  PlayerFrame();

private: // UI
  wxPanel* video_canvas_pane;
  wxMediaCtrl* media_ctrl;

  wxFileName file_name;

  gui::player::MediaControls* player_ctrls;
  gui::player::Playlist* playlist;

  gui::StatusBar* player_statusbar;

  wxLongLong media_length;
  wxLongLong current_position;

  std::size_t current_index;
  logic::Timer* tm;

  friend class gui::player::Playlist;

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
  ID_MEDIA_CTRL
};

#endif // !__WANJPLAYER__HPP
