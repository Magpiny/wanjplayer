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

#ifndef MEDIA_CTRL__HPP
#define MEDIA_CTRL__HPP

#include "widgets.hpp"
class MediaCtrl : public wxPanel {
 public:
  MediaCtrl(wxWindow* parent);
  void create_mediactrl();
  ~MediaCtrl();

 private:
  wxButton* btn_play_pause;
  wxButton* btn_stop;
  wxButton* btn_next;
  wxButton* btn_prev;

  wxSlider* slider_vol;
  wxSlider* slider_seek;

  wxStaticText* label_vol;
  wxStaticText* label_seek;

  // Event Managers
 private:
  void OnPlayPause(wxCommandEvent& event);
  void OnStop(wxCommandEvent& event);
  void OnNext(wxCommandEvent& event);
  void OnPrevious(wxCommandEvent& event);
  void OnVolumeChange(wxCommandEvent& event);
  void OnSeekChange(wxCommandEvent& event);
};

#endif
