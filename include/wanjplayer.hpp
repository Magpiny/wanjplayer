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

#ifndef WANJPLAYER__HPP
#define WANJPLAYER__HPP
#include <wx/wx.h>

#include "main_window.hpp"
#include "widgets.hpp"

class WanjPlayer : public wxApp {
 public:
  bool OnInit() override;
};

enum {
  ID_OPENFILE = wxID_HIGHEST + 1,
  ID_OPENFILES,
  ID_PREFS,
  ID_LICENSE,
  ID_MEDIA_LOADED,
  ID_MEDIA_STOP,
  ID_MEDIA_PLAY,
  ID_MEDIA_PAUSE,
  ID_MEDIA_FINISHED,
  ID_MEDIA_CANVAS
};

// wxDECLARE_APP(WanjPlayer);

#endif  // WANJPLAYER__HPP
