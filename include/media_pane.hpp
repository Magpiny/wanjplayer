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

#include "widgets.hpp"

class MediaPane : public wxPanel {
 public:
  MediaPane(wxWindow* parent);
  void create_mediapane();

 private:
  wxStaticText* mediaInfoTxt;
};

#endif