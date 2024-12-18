#ifndef __MENUBAR__HPP
#define __MENUBAR__HPP

#include "wanjplayer.hpp"

namespace gui {
class Menubar
{
public:
  Menubar(wxFrame* parent);
  void create_menubar();

private:
  wxFrame* _parent;
};
}

#endif // !__MENUBAR__H
