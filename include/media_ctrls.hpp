#ifndef __MEDIA_CONTROLS__HPP
#define __MEDIA_CONTROLS__HPP
#include "widgets.hpp"

namespace gui::player {
class MediaControls : public wxPanel
{
  wxPanel* _parent;
  wxMediaCtrl* _pmedia_ctrl;

public:
  MediaControls(wxPanel* parent, wxMediaCtrl* media_ctrl);
  // void BindEvents(wxMediaCtrl* mediactrl);

private:
  wxButton* btn_play;
  wxButton* btn_pause;
  wxButton* btn_stop;
  wxButton* btn_next;
  wxButton* btn_prev;
  wxSlider* slider_volume;

  void OnPlay(wxCommandEvent& event);
  void OnStop(wxCommandEvent& event);
  void OnPause(wxCommandEvent& event);
  void OnVolumeChange(wxScrollEvent& event);
  void OnNext(wxCommandEvent& event);
  void OnPrevious(wxCommandEvent& event);

private:
  void OnVideoCanvasHover(wxMouseEvent& event);
  void OnVideoCanvasLeave(wxMouseEvent& event);
};
}
#endif // !__MEDIA_CONTROLS__HPP
