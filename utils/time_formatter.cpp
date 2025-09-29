#include "time_formatter.hpp"
#include <wx/regex.h>
#include <cmath>

namespace utils {

wxString TimeFormatter::FormatTime(wxFileOffset milliseconds)
{
    if (milliseconds <= 0) {
        return "0s";
    }
    
    long total_seconds = milliseconds / 1000;
    long hours = total_seconds / 3600;
    long minutes = (total_seconds % 3600) / 60;
    long seconds = total_seconds % 60;
    
    wxString result = "";
    if (hours > 0) {
        result += wxString::Format("%ldh ", hours);
    }
    if (minutes > 0 || hours > 0) {
        result += wxString::Format("%ldm ", minutes);
    }
    result += wxString::Format("%lds", seconds);
    
    return result;
}

wxString TimeFormatter::FormatDuration(wxFileOffset current_ms, wxFileOffset total_ms)
{
    return FormatTime(current_ms) + " / " + FormatTime(total_ms);
}

wxString TimeFormatter::FormatTimeCompact(wxFileOffset milliseconds)
{
    if (milliseconds <= 0) {
        return "00:00:00";
    }
    
    long total_seconds = milliseconds / 1000;
    long hours = total_seconds / 3600;
    long minutes = (total_seconds % 3600) / 60;
    long seconds = total_seconds % 60;
    
    if (hours > 0) {
        return wxString::Format("%02ld:%02ld:%02ld", hours, minutes, seconds);
    } else {
        return wxString::Format("%02ld:%02ld", minutes, seconds);
    }
}

wxString TimeFormatter::FormatTimeDetailed(wxFileOffset milliseconds)
{
    if (milliseconds <= 0) {
        return "00:00:00.000";
    }
    
    long total_ms = milliseconds % 1000;
    long total_seconds = milliseconds / 1000;
    long hours = total_seconds / 3600;
    long minutes = (total_seconds % 3600) / 60;
    long seconds = total_seconds % 60;
    
    if (hours > 0) {
        return wxString::Format("%02ld:%02ld:%02ld.%03ld", hours, minutes, seconds, total_ms);
    } else {
        return wxString::Format("%02ld:%02ld.%03ld", minutes, seconds, total_ms);
    }
}

wxString TimeFormatter::FormatTimeMinimal(wxFileOffset milliseconds)
{
    if (milliseconds <= 0) {
        return "0s";
    }
    
    long total_seconds = milliseconds / 1000;
    long hours = total_seconds / 3600;
    long minutes = (total_seconds % 3600) / 60;
    long seconds = total_seconds % 60;
    
    if (hours > 0) {
        return wxString::Format("%ld:%02ld:%02ld", hours, minutes, seconds);
    } else if (minutes > 0) {
        return wxString::Format("%ld:%02ld", minutes, seconds);
    } else {
        return wxString::Format("%lds", seconds);
    }
}

wxString TimeFormatter::FormatTimeRange(wxFileOffset start_ms, wxFileOffset end_ms)
{
    return FormatTimeCompact(start_ms) + " - " + FormatTimeCompact(end_ms);
}

long TimeFormatter::MillisecondsToSeconds(wxFileOffset milliseconds)
{
    return static_cast<long>(milliseconds / 1000);
}

wxFileOffset TimeFormatter::SecondsToMilliseconds(long seconds)
{
    return static_cast<wxFileOffset>(seconds * 1000);
}

wxFileOffset TimeFormatter::ParseTimeString(const wxString& time_str)
{
    wxRegEx timeRegex;
    wxFileOffset total_ms = 0;
    
    // Try HH:MM:SS.mmm format
    if (timeRegex.Compile("^([0-9]+):([0-9]{2}):([0-9]{2})\\.([0-9]{3})$") && 
        timeRegex.Matches(time_str)) {
        
        long hours, minutes, seconds, milliseconds;
        timeRegex.GetMatch(time_str, 1).ToLong(&hours);
        timeRegex.GetMatch(time_str, 2).ToLong(&minutes);
        timeRegex.GetMatch(time_str, 3).ToLong(&seconds);
        timeRegex.GetMatch(time_str, 4).ToLong(&milliseconds);
        
        total_ms = (hours * 3600 + minutes * 60 + seconds) * 1000 + milliseconds;
    }
    // Try HH:MM:SS format
    else if (timeRegex.Compile("^([0-9]+):([0-9]{2}):([0-9]{2})$") && 
             timeRegex.Matches(time_str)) {
        
        long hours, minutes, seconds;
        timeRegex.GetMatch(time_str, 1).ToLong(&hours);
        timeRegex.GetMatch(time_str, 2).ToLong(&minutes);
        timeRegex.GetMatch(time_str, 3).ToLong(&seconds);
        
        total_ms = (hours * 3600 + minutes * 60 + seconds) * 1000;
    }
    // Try MM:SS format
    else if (timeRegex.Compile("^([0-9]+):([0-9]{2})$") && 
             timeRegex.Matches(time_str)) {
        
        long minutes, seconds;
        timeRegex.GetMatch(time_str, 1).ToLong(&minutes);
        timeRegex.GetMatch(time_str, 2).ToLong(&seconds);
        
        total_ms = (minutes * 60 + seconds) * 1000;
    }
    // Try seconds only
    else if (timeRegex.Compile("^([0-9]+)s?$") && 
             timeRegex.Matches(time_str)) {
        
        long seconds;
        timeRegex.GetMatch(time_str, 1).ToLong(&seconds);
        total_ms = seconds * 1000;
    }
    
    return total_ms;
}

bool TimeFormatter::IsValidTimeString(const wxString& time_str)
{
    return ParseTimeString(time_str) >= 0;
}

}