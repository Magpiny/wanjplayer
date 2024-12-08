#include "media_pane.hpp"

#include <vlcpp/Media.hpp>

MediaPane::MediaPane(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 650)) {
  try {
    vlc_instance = VLC::Instance(0, nullptr);
    vlc_media_player = VLC::MediaPlayer(vlc_instance);
    SetVLCPlayerHandle();
  } catch (const std::exception& e) {
    wxLogWarning(wxString::Format("Error initializing VLC Media Player:\n %s",
                                  e.what()));
  };

  Bind(wxEVT_PAINT, &MediaPane::OnPaint, this);
  Bind(wxEVT_SIZE, &MediaPane::OnResize, this);
  Bind(wxEVT_SHOW, &MediaPane::OnShow, this);
};

void MediaPane::SetVLCPlayerHandle() {
  // Confirm that VLC is properly initialised
  if (!vlc_media_player) {
    wxLogWarning("VLC Media Player not initialized");
    return;
  }
  // Set VLC Player handle to this pane
  wxString session_type;
  wxGetEnv("XDG_SESSION_TYPE", &session_type);
  wxLogDebug("Session type: %s", session_type);

  if (session_type == "wayland") {
    // For Wayland, we'll use XWayland as a fallback.
    // This might not be optimal for native Wayland, but it works for many
    // scenarios.
    wxString x11_display;
    wxGetEnv("DISPLAY", &x11_display);
    wxLogDebug("DISPLAY type: %s", x11_display);

    if (!x11_display.empty()) {
      // Force XWayland for rendering if DISPLAY is set
      vlc_media_player.setXwindow((unsigned long)this->GetHandle());
    } else {
      // Inform the user! TODO: Handle this differently
      wxLogWarning(
          "Wayland detected but no DISPLAY environment set. VLC might not work "
          "correctly");
    };
  } else if (session_type == "x11") {
// For X11, use the native window handle
#ifdef __WXMSW__
    vlc_media_player.setHwnd(this->GetHandle());  // For Windows
#elif __WXGTK__
    // Use wxWidgets' GTK-specific methods
    /* if (this->IsShown()) {*/
    /*wxWindow* wxwindow = this;*/
    /*GtkWidget* widget = (GtkWidget*)wxwindow->GetHandle();*/
    /*GdkWindow* window = gtk_widget_get_window(widget);*/
    /*vlc_media_player.setXwindow(GDK_WINDOW_XID(window));*/

    /*if (this->IsShownOnScreen()) {*/
    /*GtkWidget* widget = GTK_WIDGET(this->GetHandle());*/
    /*GdkWindow* gdk_window = gtk_widget_get_window(widget);*/
    /*if (gdk_window) {*/
    /*Window xwindow = gdk_x11_window_get_xid(gdk_window);*/
    /*vlc_media_player.setXwindow(xwindow);*/
    /*} else {*/
    /*wxLogWarning("GdkWindow is null!");*/
    /*}*/
    /*} else {*/
    /*wxLogWarning("Window is not yet realized!");*/
    /*}*/

    if (this->IsShownOnScreen()) {
      vlc_media_player.setXwindow(GDK_WINDOW_XID(gtk_widget_get_window(
          GTK_WIDGET(this->GetHandle()))));  // For Linux/GTK
      /*      vlc_media_player.setXwindow(*/
      /*gdk_x11_window_get_xid(gtk_widget_get_window(this->GetHandle())));*/
    } else {
      wxLogWarning("Window is not drawn yet!");
    }
#else
#error "Unsupported platform for X11"
#endif
  } else {
    vlc_media_player.setXwindow((unsigned long)this->GetHandle());
    wxLogWarning("Unknown session type. Defaulting to X11 handling.");
  };
};

void MediaPane::OnShow(wxShowEvent& event) {
  if (event.IsShown()) {
    SetVLCPlayerHandle();
  }
}

void MediaPane::create_mediapane() {
  // video_window = new wxWindow(this, ID_MEDIA_CANVAS, wxDefaultPosition,
  //  wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_NONE);

  media_info_txt = new wxStaticText(this, wxID_ANY, "WanjPlayer is playing",
                                    wxDefaultPosition, wxSize(400, 50));

  wxBoxSizer* text_sizer = new wxBoxSizer(wxVERTICAL);
  text_sizer->Add(media_info_txt, 1, wxALIGN_CENTRE_HORIZONTAL | wxALL, 10);
  SetSizer(text_sizer);

  this->Show(true);
  SetVLCPlayerHandle();
};

void MediaPane::Play(const wxString path) {
  auto media = VLC::Media(vlc_instance, path.ToStdString(wxConvUTF8),
                          VLC::Media::FromPath);
  vlc_media_player.setMedia(media);
  vlc_media_player.play();
};

void MediaPane::Pause() {
  // Pause media
  vlc_media_player.pause();
};

void MediaPane::Stop() {
  // Stop media player
  vlc_media_player.stop();
};

void MediaPane::OnPaint(wxPaintEvent& event) {
  // To implement paint events her
  event.Skip();
};

void MediaPane::OnResize(wxSizeEvent& event) {
  // To implement resize events here
  event.Skip();
};
