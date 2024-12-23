#ifndef __MEDIA_CONTROLS__HPP
#define __MEDIA_CONTROLS__HPP
#include "widgets.hpp"

namespace gui::player {
class MediaControls : public wxPanel
{
  wxMediaCtrl* _pmedia_ctrl;

public:
  MediaControls(wxPanel* parent, wxMediaCtrl* media_ctrl);

private:
  wxButton* btn_play;
  wxButton* btn_pause;
  wxButton* btn_stop;
  wxButton* btn_next;
  wxButton* btn_prev;

  wxSlider* slider_volume;
  wxSlider* slider_playback_position;

  void OnPlay(wxCommandEvent& event);
  void OnStop(wxCommandEvent& event);
  void OnPause(wxCommandEvent& event);
  void OnVolumeChange(wxCommandEvent& event);
  void OnNext(wxCommandEvent& event);
  void OnPrevious(wxCommandEvent& event);

private:
  void OnVideoCanvasHover(wxMouseEvent& event);
  void OnVideoCanvasLeave(wxMouseEvent& event);
};
}
#endif // !__MEDIA_CONTROLS__HPP
