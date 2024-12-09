/*
    WanjPlayer: A simple media player
Copyright (C) 2025  Wanjare Samuel

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */
#ifndef MEDIA_PANE__HPP
#define MEDIA_PANE__HPP

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

// #include <wx/gtk/window.h>  // For GTK-specific window handling

#include "vlcpp/vlc.hpp"
#include "widgets.hpp"

class MediaPane : public wxPanel {
public:
    MediaPane(wxWindow* parent);
    void create_mediapane();
    void Play(const wxString path);
    void Pause();
    void Stop();

private:
    wxStaticText* media_info_txt;
    VLC::Instance vlc_instance;
    VLC::MediaPlayer vlc_media_player;

    wxWindow* video_window;

    // Set VLC player
    void SetVLCPlayerHandle();

private:
    void OnResize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnShow(wxShowEvent& event);
};

#endif
