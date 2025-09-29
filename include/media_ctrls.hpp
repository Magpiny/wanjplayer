#ifndef __MEDIA_CONTROLS__HPP
#define __MEDIA_CONTROLS__HPP
#include "widgets.hpp"

namespace gui {
class StatusBar; // Forward declaration
}

namespace gui::player {
class Playlist; // Forward declaration

class MediaControls : public wxPanel
{
  wxMediaCtrl* _pmedia_ctrl;

public:
  MediaControls(wxPanel* parent, wxMediaCtrl* media_ctrl);
  ~MediaControls();
  void UpdateDuration();
  void SetPlaylist(Playlist* playlist);
  void SetStatusBar(gui::StatusBar* status_bar);

private:
  wxButton* btn_play;
  wxButton* btn_pause;
  wxButton* btn_stop;
  wxButton* btn_next;
  wxButton* btn_prev;

  wxSlider* slider_volume;
  wxSlider* slider_playback_position;
  
  // Duration display
  wxStaticText* label_current_time;
  wxStaticText* label_total_time;
  wxStaticText* label_separator;
  
  // Timer for updating playback position
  wxTimer* update_timer;
  
  // Playlist reference for navigation
  Playlist* playlist;
  
  // Status bar reference for updates
  gui::StatusBar* status_bar;
  
  // Media duration and position tracking
  wxFileOffset media_duration;
  wxFileOffset media_position;

  void OnPlay(wxCommandEvent& event);
  void OnStop(wxCommandEvent& event);
  void OnPause(wxCommandEvent& event);
  void OnVolumeChange(wxCommandEvent& event);
  void OnNext(wxCommandEvent& event);
  void OnPrevious(wxCommandEvent& event);
  void OnPositionSliderChange(wxCommandEvent& event);
  void OnPositionSliderSeek(wxMouseEvent& event);
  void OnUpdateTimer(wxTimerEvent& event);

private:
  void OnVideoCanvasHover(wxMouseEvent& event);
  void OnVideoCanvasLeave(wxMouseEvent& event);
  
  // Helper methods
  wxString FormatTime(wxFileOffset milliseconds);
  void UpdateTimeDisplay();
  void UpdatePositionSlider();
};
}
#endif // !__MEDIA_CONTROLS__HPP
