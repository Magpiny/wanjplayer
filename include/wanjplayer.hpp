#ifndef __WANJPLAYER__HPP
#define __WANJPLAYER__HPP

#include "wx_widgets.hpp"
#include <wx/wx.h>

class WanjPlayer : public wxApp
{
public:
  bool OnInit() override;
};

class PlayerFrame : public wxFrame
{
public:
  PlayerFrame();

private:
  wxMediaCtrl* media_ctrl;

  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnFileOpen(wxCommandEvent& event);
  void OnLicense(wxCommandEvent& event);
  void OnPreferences(wxCommandEvent& event);
  void OnMediaLoaded(wxCommandEvent& event);
};

enum
{
  ID_OPENFILE = 100,
  ID_PREFS,
  ID_LICENSE,
  ID_MEDIA_LOADED
};

#endif // !__WANJPLAYER__HPP
