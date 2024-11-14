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
  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
};

enum
{
  ID_Hello = 1
};

#endif // !__WANJPLAYER__H
