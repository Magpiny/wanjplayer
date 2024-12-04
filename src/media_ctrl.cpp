#include "media_ctrl.hpp"

MediaCtrl::MediaCtrl(wxWindow *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(800, 100),
              wxNO_BORDER | wxTRANSPARENT_WINDOW) {
  Bind(wxEVT_BUTTON, &MediaCtrl::OnPlayPause, this, btn_play_pause->GetId());
  Bind(wxEVT_BUTTON, &MediaCtrl::OnStop, this, btn_stop->GetId());
  Bind(wxEVT_BUTTON, &MediaCtrl::OnNext, this, btn_next->GetId());
  Bind(wxEVT_BUTTON, &MediaCtrl::OnPrevious, this, btn_prev->GetId());
  Bind(wxEVT_SLIDER, &MediaCtrl::OnVolumeChange, this, slider_vol->GetId());
  Bind(wxEVT_SLIDER, &MediaCtrl::OnSeekChange, this, slider_seek->GetId());
};

void MediaCtrl::create_mediactrl() {
  // Setup UI
  btn_play_pause =
      new wxButton(this, wxID_ANY, "Play", wxDefaultPosition, wxSize(70, 30));
  btn_stop =
      new wxButton(this, wxID_ANY, "Stop", wxDefaultPosition, wxSize(70, 30));
  btn_next =
      new wxButton(this, wxID_ANY, "Next", wxDefaultPosition, wxSize(70, 30));
  btn_prev = new wxButton(this, wxID_ANY, "Previous", wxDefaultPosition,
                          wxSize(70, 30));
  slider_vol = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition,
                            wxSize(150, -1));
  slider_seek = new wxSlider(this, wxID_ANY, 0, 0, 100, wxDefaultPosition,
                             wxSize(300, -1));
  label_vol = new wxStaticText(this, wxID_ANY, "Volume");
  label_seek = new wxStaticText(this, wxID_ANY, "Seek");

  // Layout
  wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *slider_sizer = new wxBoxSizer(wxHORIZONTAL);

  btn_sizer->Add(btn_play_pause, 0, wxALL, 5);
  btn_sizer->Add(btn_stop, 0, wxALL, 5);
  btn_sizer->Add(btn_next, 0, wxALL, 5);
  btn_sizer->Add(btn_prev, 0, wxALL, 5);

  slider_sizer->Add(label_vol, 0, wxALL, 5);
  slider_sizer->Add(slider_vol, 1, wxEXPAND | wxALL, 5);
  slider_sizer->Add(label_seek, 0, wxALL, 5);
  slider_sizer->Add(slider_seek, 3, wxEXPAND | wxALL, 5);

  main_sizer->Add(btn_sizer, 0, wxEXPAND | wxALL, 5);
  main_sizer->Add(slider_sizer, 0, wxEXPAND | wxALL, 5);

  SetSizer(main_sizer);
  SetTransparent(200);  // Adjust transparency
};

MediaCtrl::~MediaCtrl() {};
