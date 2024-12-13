
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
#ifndef STATUSBAR__HPP
#define STATUSBAR__HPP

#include "widgets.hpp"
class StatusBar : public wxFrame
{
    public:
        StatusBar(wxFrame* parent);
        void create_statusbar(wxString mssg1, wxString mssg2, wxString mssg3);
        ~StatusBar();

    private:
        wxFrame* _parent;
};

#endif // !STATUSBAR__HPP
