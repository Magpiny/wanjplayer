// Another file by Wanjman
#include "media_ctrls.hpp"
#include "player_ui.hpp"
#include "wanjplayer.hpp"

gui::player::MediaControls::MediaControls(wxPanel* panel,
                                          wxMediaCtrl* media_ctrl)
  : wxPanel(panel, wxID_ANY, wxDefaultPosition, wxSize(600, 80))
  , _pmedia_ctrl(media_ctrl)
{

  SetBackgroundColour(wxTransparentColour);

  // Create the three buttons
  btn_play = new wxButton(this, wxID_ANY, "play");
  btn_stop = new wxButton(this, wxID_ANY, "stop");
  btn_pause = new wxButton(this, wxID_ANY, "pause");
  // btn_next = new wxButton(this, wxID_ANY, "next");
  // btn_prev = new wxButton(this, wxID_ANY, "prev");

  // Add a volume slider
  slider_volume = new wxSlider(this, wxID_ANY, 50, 0, 100);

  // Create a horizontal box sizer for the buttons and slider
  // mc ---> media controls
  wxBoxSizer* mc_sizer = new wxBoxSizer(wxHORIZONTAL);
  mc_sizer->Add(btn_play, 0, wxALL | wxCENTER, 4);
  mc_sizer->Add(btn_stop, 0, wxALL | wxCENTER, 4);
  mc_sizer->Add(btn_pause, 0, wxALL | wxCENTER, 4);
  // mc_sizer->Add(btn_next, 0, wxALL | wxCENTER, 4);
  // mc_sizer->Add(btn_prev, 0, wxALL | wxCENTER, 4);
  mc_sizer->Add(slider_volume, 1, wxALL | wxCENTER, 4);

  // Set sizer for this media_control panel
  SetSizerAndFit(mc_sizer);
  Layout();
  Raise();

  // Bind events
  Bind(wxEVT_BUTTON, &MediaControls::OnPlay, this, btn_play->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnStop, this, btn_stop->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnPause, this, btn_pause->GetId());
  Bind(wxEVT_SCROLL_CHANGED,
       &MediaControls::OnVolumeChange,
       this,
       slider_volume->GetId());

  // Bind hover events
  Bind(wxEVT_ENTER_WINDOW,
       &MediaControls::OnVideoCanvasHover,
       this,
       ID_MEDIA_CANVAS);
  Bind(wxEVT_LEAVE_WINDOW,
       &MediaControls::OnVideoCanvasLeave,
       this,
       ID_MEDIA_CANVAS);
}

void
gui::player::MediaControls::OnVideoCanvasHover(wxMouseEvent& event)
{
  this->Show();
  this->Refresh();
  event.Skip();
}

void
gui::player::MediaControls::OnVideoCanvasLeave(wxMouseEvent& event)
{
  this->Hide();
  this->Refresh();
  event.Skip();
}
