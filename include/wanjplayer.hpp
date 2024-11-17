#ifndef __WANJPLAYER__H
#define __WANJPLAYER__H

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

#endif // !__WANJPLAYER__H
