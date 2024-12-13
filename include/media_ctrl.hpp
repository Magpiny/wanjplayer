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

namespace MCDimensions
{
const int btn1_length = 70;
const int btn2_length = 90;
const int btn1_width = 30;
const int pane_length = 600;
const int pane_width = 40;

// Sizers dimenension
const int sz_vol_start = 0;
const int sz_vol_end = 100;
const int sz_vol_curr = 45;

const int sz_seek_start = 0;
const int sz_seek_msize = 100;
const int sz_seek_curr = 0;
}; // namespace MCDimensions

class MediaCtrl : public wxPanel
{
    public:
        MediaCtrl(wxWindow* parent);
        void create_mediactrl();

    private:
        // Colour DB
        wxColourDatabase m_color;

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
