#ifndef __STATUSBAR__HPP
#define __STATUSBAR__HPP

#include "wanjplayer.hpp"

namespace gui {

class StatusBar
{
public:
  StatusBar(wxFrame* parent);
  void create_statusbar(wxString mssg1, wxString mssg2, wxString mssg3);
  ~StatusBar() {};

private:
  wxFrame* _parent;
};

}

#endif // !__STATUSBAR__HPP
