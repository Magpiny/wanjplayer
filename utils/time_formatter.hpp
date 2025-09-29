#ifndef __TIME_FORMATTER_HPP
#define __TIME_FORMATTER_HPP

#include <wx/wx.h>
#include <wx/mediactrl.h>

namespace utils {

// Time formatting utilities
class TimeFormatter {
public:
    static wxString FormatTime(wxFileOffset milliseconds);
    static wxString FormatDuration(wxFileOffset current_ms, wxFileOffset total_ms);
    static wxString FormatTimeCompact(wxFileOffset milliseconds); // HH:MM:SS format
    static long MillisecondsToSeconds(wxFileOffset milliseconds);
    static wxFileOffset SecondsToMilliseconds(long seconds);
    
    // Additional formatting options
    static wxString FormatTimeDetailed(wxFileOffset milliseconds); // With milliseconds
    static wxString FormatTimeMinimal(wxFileOffset milliseconds); // Shortest possible format
    static wxString FormatTimeRange(wxFileOffset start_ms, wxFileOffset end_ms);
    
    // Parse time strings back to milliseconds
    static wxFileOffset ParseTimeString(const wxString& time_str);
    
    // Validation
    static bool IsValidTimeString(const wxString& time_str);
};

}

#endif // __TIME_FORMATTER_HPP