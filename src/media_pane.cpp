#include "media_pane.hpp"

#include <wx/aboutdlg.h>
#include <wx/log.h>

#include <vlcpp/Media.hpp>

MediaPane::MediaPane(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(600, 650))
{
    video_window = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL | wxBORDER_NONE);

    if (!video_window->IsShown()) {
        video_window->Show();

        try {
            vlc_instance = VLC::Instance(0, nullptr);
            vlc_media_player = VLC::MediaPlayer(vlc_instance);
            SetVLCPlayerHandle();
        } catch (const std::exception& e) {
            wxLogWarning(wxString::Format("Error initializing VLC Media Player:\n %s",
                e.what()));
        };

    } else {

        try {
            vlc_instance = VLC::Instance(0, nullptr);
            vlc_media_player = VLC::MediaPlayer(vlc_instance);
            SetVLCPlayerHandle();
        } catch (const std::exception& e) {
            wxLogWarning(wxString::Format("Error initializing VLC Media Player:\n %s",
                e.what()));
        };
    }

    Bind(wxEVT_PAINT, &MediaPane::OnPaint, this);
    Bind(wxEVT_SIZE, &MediaPane::OnResize, this);
    Bind(wxEVT_SHOW, &MediaPane::OnShow, this);
};

void MediaPane::SetVLCPlayerHandle()
{
#ifdef __WXMSW__
    vlc_media_player.setHwnd(video_window->GetHandle()); // For Windows
#endif
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
            // vlc_media_player.setXwindow((unsigned long)video_window->GetHandle());
            vlc_media_player.setXwindow((unsigned long)(video_window->GetHandle()));
        } else {
            // Inform the user! TODO: Handle this differently
            wxLogWarning(
                "Wayland detected but no DISPLAY environment set. VLC might not work "
                "correctly");
        };
    } else if (session_type == "x11") {
// For X11, use the native window handle
#ifdef __WXGTK__
        // Use wxWidgets' GTK-specific methods

        vlc_media_player.setXwindow(gdk_x11_window_get_xid(
            gtk_widget_get_window(video_window->GetHandle())));
#else
#error "Unsupported platform for X11"
#endif
    } else {
        vlc_media_player.setXwindow((unsigned long)video_window->GetHandle());
        wxLogWarning("Unknown session type. Defaulting to X11 handling.");
    };
};

void MediaPane::OnShow(wxShowEvent& event)
{
    if (!event.IsShown()) {
        video_window->Show();
        SetVLCPlayerHandle();
    }
}

void MediaPane::Play(const wxString& path)
{
    auto media = VLC::Media(vlc_instance, path.ToStdString(wxConvUTF8),
        VLC::Media::FromPath);
    vlc_media_player.setMedia(media);
    vlc_media_player.play();
};

void MediaPane::Pause()
{
    // Pause media
    vlc_media_player.pause();
};

void MediaPane::Stop()
{
    // Stop media player
    vlc_media_player.stop();
};

void MediaPane::OnPaint(wxPaintEvent& event)
{
    // To implement paint events her
    event.Skip();
};

void MediaPane::OnResize(wxSizeEvent& event)
{
    // To implement resize events here
    event.Skip();
};
