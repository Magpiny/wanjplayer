#include "media_pane.hpp"

MediaPane::MediaPane(wxWindow *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 650)) {};

void MediaPane::create_mediapane() {
  vlc_instance = VLC::Instance(0, nullptr);
  vlc_media_player = VLC::MediaPlayer(vlc_instance);

  media_info_txt = new wxStaticText(this, wxID_ANY, "WanjPlayer",
                                    wxDefaultPosition, wxSize(400, 50));

  wxBoxSizer *text_sizer = new wxBoxSizer(wxVERTICAL);
  text_sizer->Add(media_info_txt, 1, wxALIGN_CENTRE_HORIZONTAL | wxALL, 10);
  SetSizer(text_sizer);
};
