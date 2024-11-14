#ifndef __PLAYER__H
#define __PLAYER__H

namespace player {
class Controls
{
  Controls() {};
  void pause();
  void next();
  void stop();
  void previous();
  ~Controls() {};
};
}
#endif // !__PLAYER__H
