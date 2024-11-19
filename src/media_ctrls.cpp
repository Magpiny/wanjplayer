#include "player_ui.hpp"

gui::player::MediaControls::MediaControls(wxPanel* panel)
  : wxPanel(this, wxID_ANY)
{
  aui_manager = new wxAuiManager(this);

  SetBackgroundColour(wxTransparentColour);

  // Create the three buttons
  btn_play = new wxButton(this, wxID_ANY, "play");
  btn_stop = new wxButton(this, wxID_ANY, "stop");
  btn_pause = new wxButton(this, wxID_ANY, "pause");
  btn_next = new wxButton(this, wxID_ANY, "next");
  btn_prev = new wxButton(this, wxID_ANY, "prev");

  // Add a volume slider
  slider_volume = new wxSlider(this, wxID_ANY, 50, 0, 100);

  // Create a horizontal box sizer for the buttons and slider
  // mc ---> media controls
  wxBoxSizer mc_sizer = new wxBoxSizer(wxHORIZONTAL);
  mc_sizer->Add(btn_play, 0, wxALL | wxCENTER, 4);
  mc_sizer->Add(btn_stop, 0, wxALL | wxCENTER, 4);
  mc_sizer->Add(btn_pause, 0, wxALL | wxCENTER, 4);
  // mc_sizer->Add(btn_next, 0, wxALL | wxCENTER, 4);
  // mc_sizer->Add(btn_prev, 0, wxALL | wxCENTER, 4);
  mc_sizer->Add(slider_volume, 1, wxALL | wxCENTER, 4);

  // Set sizer for this media_control panel
  SetSizer(mc_sizer);
  Layout();

  // Bind events
  Bind(wxEVT_BUTTON, &MediaControls::OnPlay, this, btn_play->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnStop, this, btn_stop->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnPause, this, btn_pause->GetId());
  Bind(
    wxEVT_SLIDER, &MediaControls::OnVolumeChange, this, slider_volume->GetId());
}

void
gui::player::MediaControls::OnVideoCanvasHover(wxMouseEvent& event)
{
  mediaCtrls->Show();
  mediaCtrls->Refresh();
  event.Skip();
}

void
gui::player::MediaControls::OnVideoCanvasLeave(wxMouseEvent& event)
{
  mediaCtrls->Hide();
  mediaCtrls->Refresh();
  event.Skip();
}

void
gui::player::MediaControls::OnPlay(wxCommandEvent& event)
{
  media_ctrl->Play();
};

void
gui::player::MediaControls::OnStop(wxCommandEvent& event)
{
  media_ctrl->Stop();
};

void
gui::player::MediaControls::Onpause(wxCommandEvent& event)
{
  media_ctrl->Pause();
};

void
gui::player::MediaControls::OnVolumeChange(wxScrollEvent& event) {};

void
gui::player::MediaControls::OnNext(wxCommandEvent& event) {};

void
gui::player::MediaControls::OnPrevious(wxCommandEvent& event) {};

gui::player::~MediaControls()
{
  delete btn_play;
  delete btn_stop;
  delete btn_pause;
  delete btn_next;
  delete btn_prev;
  delete slider_volume;
  delete mc_sizer;
}
