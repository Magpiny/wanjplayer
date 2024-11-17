#ifndef __STATUSBAR__HPP
#define __STATUSBAR__HPP

#include "wanjplayer.hpp"

namespace gui {

class StatusBar
{
public:
  StatusBar(wxFrame* parent);
  void create_statusbar(wxT mssg1, wxT mssg2, wxT mssg3);
  ~StatusBar() {};

private:
  wxFrame* _parent;
};

}

#endif // !__STATUSBAR__HPP
