/*
 * Author: Wanjare Samuel
 * Date: 29th Nov 2024 15:39pm EAT
 *
 * APP TIMER CLASS: All timing calculations and events will be done here
 *
 * METHOD 1: A method to convert time that will be shown in the status bar
 * It may also have some timing functions in future
 */

#include <sstream>
#include <string>
#include <vector>
#include <wx/filefn.h>
#include <wx/longlong.h>
#include <wx/string.h>

namespace logic {

struct TimeConstants
{
  static constexpr int SECOND = 1000; // 1000 milliseconds
  static constexpr int MINUTE = 60 * SECOND;
  static constexpr int HOUR = 60 * MINUTE;
  static constexpr int DAY = 24 * HOUR;
};

class Timer
{
  //  wxString mystring(timer_string: ie 3hr:45min: 45sec);  <- I wanna return
  //  this
public:
  Timer();
  wxString time_convert(wxLongLong m_time); // Takes int time in milliseconds
  ~Timer();
};
}
