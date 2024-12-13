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

// A method to convert time that will be shown in the status bar
// It may also have some timing functions in future

#ifndef WANJ_TIMER__HPP
#define WANJ_TIMER__HPP

#include <wx/longlong.h>

#include <iostream>
namespace logic::player
{

struct TimeConstants {
        static constexpr int SECOND = 1000;
        static constexpr int MINUTE = 60 * SECOND;
        static constexpr int HOUR = 60 * MINUTE;
        static constexpr int DAY = 24 * HOUR;
};

class Timer
{
        Timer();
        wxString time_convert(wxLongLong time); // Takes int time in milliseconds
        ~Timer();
};
} // namespace logic::player

#endif // !WANJ_TIMER__HPP
