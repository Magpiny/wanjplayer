#include "w_timer.hpp"

logic::Timer::Timer() {}

wxString
logic::Timer::time_convert(wxLongLong m_time)
{
  // Calculate the number of days, hours, minutes, and seconds
  int days = (int)(m_time / TimeConstants::DAY).GetValue();
  m_time = (int)(m_time % TimeConstants::DAY).GetValue();
  int hours = (int)(m_time / TimeConstants::HOUR).GetValue();
  m_time = (int)(m_time % TimeConstants::HOUR).GetValue();
  int minutes = (int)(m_time / TimeConstants::MINUTE).GetValue();
  m_time = (int)(m_time % TimeConstants::MINUTE).GetValue();
  int seconds = (int)(m_time / TimeConstants::SECOND).GetValue();

  // Create a vector to hold the time components
  std::vector<wxString> time_components;

  if (days > 0) {
    time_components.push_back(wxString::Format("%2i d", days));
  }
  if (hours > 0) {
    time_components.push_back(wxString::Format("%2i h", hours));
  }
  if (minutes > 0) {
    time_components.push_back(wxString::Format("%2i m", minutes));
  }
  if (seconds > 0 || time_components.empty()) {
    // Include seconds if it's the only component
    time_components.push_back(wxString::Format("%02i s", seconds));
  }

  // Join the time components with spaces
  std::ostringstream oss;
  for (size_t i = 0; i < time_components.size(); ++i) {
    if (i > 0) {
      oss << " ";
    }
    oss << time_components[i];
  }
  return wxString(oss.str());
}

logic::Timer::~Timer() {};
