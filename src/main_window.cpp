
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

#include "main_window.hpp"

MainWindow::MainWindow(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "WanjPlayer", wxDefaultPosition,
              wxSize(800, 650)) {
  // Set Icon here

  // Set window manager here i.e wxAuiManager
  _aui_mgr = new wxAuiManager();
  _aui_mgr->SetManagedWindow(this);

  // SET APP UI COMPONENTS HERE
  // Menu bar
  _menu_bar = new MenuBar(this);
  _menu_bar->create_menubar();

  /* APP UI */
  // Our App has a sidebar(left) and main window (left)
  // plus a floating buttons pane on the right window

  // Main App Window
  _side_bar = new SideBar(this);
  _side_bar->create_sidebar();

  _media_pane = new MediaPane(this);
  _media_pane->create_mediapane();

  _media_ctrl = new MediaCtrl(this);
  _media_ctrl->create_mediactrl();

  // MANAGE WINDOWS
  _aui_mgr->AddPane(_side_bar,
                    wxAuiPaneInfo().Name("Playlist").Left().Layer(1));
  _aui_mgr->AddPane(_media_pane,
                    wxAuiPaneInfo().Name("Media").Right().Layer(1));
  _aui_mgr->AddPane(_media_ctrl,
                    wxAuiPaneInfo().Name("Controls").Right().Bottom().Layer(2));

  _aui_mgr->Update();
  // CREATE STATUS BAR
  _status_bar = new StatusBar(this);
  _status_bar->create_statusbar("Hello", "Hello", "Hello");

  // Bind Events
  Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);

  Bind(wxEVT_ENTER_WINDOW, &MainWindow::OnMouseEnter, this, wxID_ANY, wxID_ANY);
  Bind(wxEVT_LEAVE_WINDOW, &MainWindow::OnMouseLeave, this, wxID_ANY, wxID_ANY);
};

MainWindow::~MainWindow() {};
