#include "w_timer.hpp"

logic::Timer::Timer() {}

wxString
logic::Timer::time_convert(wxFileOffset m_time)
{
  // Calculate the number of days, hours, minutes, and seconds
  wxFileOffset days = m_time / TimeConstants::DAY;
  m_time %= TimeConstants::DAY;
  wxFileOffset hours = m_time / TimeConstants::HOUR;
  m_time %= TimeConstants::HOUR;
  wxFileOffset minutes = m_time / TimeConstants::MINUTE;
  m_time %= TimeConstants::MINUTE;
  wxFileOffset seconds = m_time / TimeConstants::SECOND;

  // Create a vector to hold the time components
  std::vector<std::string> time_components;

  if (days > 0) {
    time_components.push_back(wxString::Format("%lld", days));
  }
  if (hours > 0) {
    time_components.push_back(wxString::Format("%lld h", hours));
  }
  if (minutes > 0) {
    time_components.push_back(wxString::Format("%lld m", minutes));
  }
  if (seconds > 0 || time_components.empty()) {
    // Include seconds if it's the only component
    time_components.push_back(wxString::Format("%lld s", seconds));
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
