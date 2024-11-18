#ifndef __PLAYER__H
#define __PLAYER__H

#include "wanjplayer.hpp"

namespace gui::player {
class Controls
{
public:
  Controls() {};
  void pause();
  void next();
  void stop();
  void previous();
  ~Controls() {};
};
}
#endif // !__PLAYER__H
