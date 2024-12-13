
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
#ifndef MAIN_WINDOW__HPP
#define MAIN_WINDOW__HPP

#include <wx/wx.h>

#include "media_ctrl.hpp"
#include "media_pane.hpp"
#include "menubar.hpp"
#include "sidebar.hpp"
#include "statusbar.hpp"
#include "timer_calc.hpp"
#include "widgets.hpp"

// Forward declaration
class MenuBar;

class MainWindow : public wxFrame
{
    public:
        MainWindow(wxWindow* parent);
        void OnMouseEnter(wxMouseEvent& event);
        void OnMouseLeave(wxMouseEvent& event);
        ~MainWindow();

    private:
        int window_width = GetSize().GetWidth();
        int sidebar_width = (window_width * 25) / 100;
        int mediapane_width = (window_width * 75) / 100;

    private:
        wxAuiManager* _aui_mgr;

        MenuBar* _menu_bar;
        SideBar* _side_bar;
        MediaPane* _media_pane;
        MediaCtrl* _media_ctrl;
        StatusBar* _status_bar;

        // Event handlers
    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        /* void OnHelp(wxCommandEvent& event);*/

        void OnFileOpen(wxCommandEvent& event);
        /*void OnFilesOpen(wxCommandEvent& event);*/
        /*void OnLicense(wxCommandEvent& event);*/
        /*void OnPreferences(wxCommandEvent& event);*/
};

#endif
