#ifndef __UTILS__HPP
#define __UTILS__HPP

// Main utilities header - includes all utility modules
#include "time_formatter.hpp"
#include "file_utils.hpp"
#include "queue_manager.hpp"
#include "performance_utils.hpp"
#include "gui_utils.hpp"
#include "log_utils.hpp"
#include "string_utils.hpp"

namespace utils {

// Convenience aliases for commonly used classes
using TimeFormatter = TimeFormatter;
using FileUtils = FileUtils;
using QueueManager = QueueManager;
using PerformanceUtils = PerformanceUtils;
using GuiUtils = GuiUtils;
using LogUtils = LogUtils;
using StringUtils = StringUtils;

// Utility initialization and cleanup
class UtilsManager {
public:
    static void Initialize();
    static void Cleanup();
    static bool IsInitialized();
    
    // Global settings
    static void SetLogLevel(LogUtils::LogLevel level);
    static void EnablePerformanceLogging(bool enable);
    static void SetTempDirectory(const wxString& directory);
    
private:
    static bool initialized;
};

// Quick access functions for most commonly used utilities
namespace quick {
    // Time formatting shortcuts
    inline wxString FormatTime(wxFileOffset ms) {
        return TimeFormatter::FormatTime(ms);
    }
    
    inline wxString FormatDuration(wxFileOffset current, wxFileOffset total) {
        return TimeFormatter::FormatDuration(current, total);
    }
    
    // File utilities shortcuts
    inline bool IsMediaFile(const wxString& path) {
        return FileUtils::IsMediaFile(path);
    }
    
    inline bool IsVideoFile(const wxString& path) {
        return FileUtils::IsVideoFile(path);
    }
    
    inline bool IsAudioFile(const wxString& path) {
        return FileUtils::IsAudioFile(path);
    }
    
    inline wxString GetFileName(const wxString& path) {
        return FileUtils::GetFileName(path);
    }
    
    // Logging shortcuts
    inline void LogInfo(const wxString& message) {
        LogUtils::LogInfo(message);
    }
    
    inline void LogError(const wxString& message) {
        LogUtils::LogError(message);
    }
    
    inline void LogWarning(const wxString& message) {
        LogUtils::LogWarning(message);
    }
}

}

#endif // __UTILS__HPP