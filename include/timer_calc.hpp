// A method to convert time that will be shown in the status bar
// It may also have some timing functions in future
#include <iostream>
namespace logic::player {

struct TimeConstants
{
  static constexpr int64_t SECOND = 1000;
  static constexpr int64_t MINUTE = 60 * SECOND;
  static constexpr int64_t HOUR = 60 * MINUTE;
  static constexpr int64_t DAY = 24 * HOUR;
};

class Timer
{
  Timer();
  wxString time_convert(off_t time); // Takes int time in milliseconds
  ~Timer();
};
}
