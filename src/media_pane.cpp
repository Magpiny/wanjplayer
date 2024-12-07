#include "media_pane.hpp"

#include <vlcpp/Media.hpp>

MediaPane::MediaPane(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 650)) {
  Bind(wxEVT_PAINT, &MediaPane::OnPaint, this);
  Bind(wxEVT_RESIZE, &MediaPane::OnResize, this);
};

void MediaPane::create_mediapane() {
  vlc_instance = VLC::Instance(0, nullptr);
  vlc_media_player = VLC::MediaPlayer(vlc_instance);

  video_window = new wxWindow(this, ID_MEDIA_CANVAS, wxDefaultPosition,
                              wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_NONE);

  media_info_txt = new wxStaticText(this, wxID_ANY, "WanjPlayer is playing",
                                    wxDefaultPosition, wxSize(400, 50));

  wxBoxSizer* text_sizer = new wxBoxSizer(wxVERTICAL);
  text_sizer->Add(media_info_txt, 1, wxALIGN_CENTRE_HORIZONTAL | wxALL, 10);
  SetSizer(text_sizer);
};

void MediaPane::OnPlay(const wxString path) {
  auto media = VLC::Media(vlc_instance, wxString::ToStdString(url));
  vlc_media_player->set_media(media);
  vlc_media_player->play();
};

void MediaPane::OnPause() {
  // Pause media
  vlc_media_player->pause();
};

void MediaPane::OnStop() {
  // Stop media player
  vlc_media_player->stop();
};

void MediaPane::OnPaint(wxPaintEvent& event) {
  // To implement paint events her
  event.Skip();
};

void MediaPane::OnResize(wxSizeEvent& event) {
  // To implement resize events here
  event.Skip();
};
