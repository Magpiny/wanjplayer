
/*
WanjPlayer: A simple media player
    Copyright (C) 2024  Wanjare Samuel

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
#ifndef SIDE_BAR__HPP
#define SIDE_BAR__HPP

#include "widgets.hpp"
class SideBar : public wxPanel {
 public:
  SideBar(wxWindow* parent);
  void create_sidebar();

 private:
  wxButton* btn_clear_playlist;
  wxButton* btn_rmv_track;
  wxButton* btn_add_track;

  wxListCtrl* list_ctrl;

  // Events
 private:
  void OnAddTrack(wxCommandEvent& event);
  void OnRemoveTrack(wxCommandEvent& event);
};

#endif  // SIDE_BAR__HPP
