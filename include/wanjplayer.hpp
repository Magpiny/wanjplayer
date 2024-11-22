#ifndef __WANJPLAYER__HPP
#define __WANJPLAYER__HPP

#include "player_ui.hpp"
#include "playlist.hpp"
#include "widgets.hpp"
#include <cstddef>
#include <memory>
#include <wx/wx.h>

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
  wxImage* default_image;
  wxImage* audio_banner;
  wxStaticBitmap* canvas_banner;

  wxPanel* video_canvas_pane;
  wxMediaCtrl* media_ctrl;

  gui::player::MediaControls* player_ctrls;
  gui::Playlist* playlist;

  wxAuiManager* aui_mgr;
  std::size_t current_index;

private: // Events
         // UI Events
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnFileOpen(wxCommandEvent& event);
  void OnFilesOpen(wxCommandEvent& event);
  void OnLicense(wxCommandEvent& event);
  void OnPreferences(wxCommandEvent& event);

  // Media Events
  void OnMediaLoaded(wxCommandEvent& event);
  void OnMediaPlay(wxCommandEvent& event);
  void OnMediaPause(wxCommandEvent& event);
  void OnMediaStop(wxCommandEvent& event);
  void OnMediaUnload(wxCommandEvent& event);
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
  ID_MEDIA_CANVAS
};

#endif // !__WANJPLAYER__HPP
