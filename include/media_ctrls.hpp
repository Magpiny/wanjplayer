#ifndef __PLAYER__H
#define __PLAYER__H

#include "wanjplayer.hpp"

namespace gui::player {
class MediaControls::wxPanel
{
public:
  MediaControls(wxWindow* parent);
  // void BindEvents(wxMediaCtrl* mediactrl);

private:
  wxButton btn_play;
  wxButton btn_pause;
  wxButton btn_stop;
  wxButton btn_next;
  wxButton btn_prev;
  wxSlider slider_volume;

  wxAuiManger aui_manager;

  void OnPlay(wxCommandEvent& event);
  void OnStop(wxCommandEvent& event);
  void OnPause(wxCommandEvent& event);
  void OnVolumeChange(wxScrollEvent& event);
  void OnNext(wxCommandEvent& event);
  void OnPrevious(wxCommandEvent& event);

  void OnVideoCanvasHover(wxMouseEvent& event);
  void OnVideoCanvasLeave(wxMouseEvent& event);

  ~MediaControls();
};
}
#endif // !__PLAYER__H
