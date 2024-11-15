#ifndef __STATUSBAR__HPP
#define __STATUSBAR__HPP

#include "wanjplayer.hpp"

namespace gui {

class StatusBar
{
public:
  StatusBar(wxFrame* parent);
  void create_statusbar();
  ~StatusBar() {};

private:
  wxFrame* _parent;
};

}

#endif // !__STATUSBAR__HPP
