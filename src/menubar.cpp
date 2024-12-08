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

#include "menubar.hpp"

MenuBar::MenuBar(wxFrame *parent) : _parent(parent) {};

void MenuBar::create_menubar() {
  wxMenu *menu_file = new wxMenu;
  menu_file->Append(wxID_OPEN);
  menu_file->Append(ID_OPENFILES, "&OpenFiles\tCtrl-Shift-O");
  menu_file->AppendSeparator();
  menu_file->Append(wxID_EXIT);

  wxMenu *menu_help = new wxMenu;
  menu_help->Append(ID_PREFS, "&Preferences");
  menu_help->Append(wxID_ABOUT);
  menu_help->Append(ID_LICENSE, "&Licence", "Read License");

  wxMenuBar *menuBar = new wxMenuBar(wxMB_DOCKABLE);
  menuBar->Append(menu_file, "&File");
  menuBar->Append(menu_help, "&Help");

  _parent->SetMenuBar(menuBar);
}
