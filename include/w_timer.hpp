/*
 * Author: Wanjare Samuel
 * Date: 29th Nov 2024 15:39pm EAT
 *
 * APP TIMER CLASS: All timing calculations and events will be done here
 *
 * METHOD 1: A method to convert time that will be shown in the status bar
 * It may also have some timing functions in future
 */

#include "widgets.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace logic {

struct TimeConstants
{
  static constexpr wxFileOffset SECOND = 1000; // 1000 milliseconds
  static constexpr wxFileOffset MINUTE = 60 * SECOND;
  static constexpr wxFileOffset HOUR = 60 * MINUTE;
  static constexpr wxFileOffset DAY = 24 * HOUR;
};

class Timer
{
  //  wxString mystring(timer_string: ie 3hr:45min: 45sec);  <- I wanna return
  //  this
public:
  Timer();
  wxString time_convert(wxFileOffset m_time); // Takes int time in milliseconds
  ~Timer();
};
}
