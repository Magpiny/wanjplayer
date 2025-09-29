#ifndef __LOG_UTILS_HPP
#define __LOG_UTILS_HPP

#include <wx/wx.h>
#include <wx/log.h>
#include <wx/datetime.h>
#include <fstream>
#include <memory>

namespace utils {

// Logging and debugging utilities
class LogUtils {
public:
    enum class LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };
    
    // Configuration
    static void Initialize(const wxString& log_file = wxEmptyString);
    static void Shutdown();
    static void SetLogLevel(LogLevel level);
    static LogLevel GetLogLevel();
    static void EnableConsoleOutput(bool enable);
    static void EnableFileOutput(bool enable, const wxString& filename = wxEmptyString);
    static void EnableTimestamp(bool enable);
    static void SetMaxLogFileSize(size_t max_size_bytes);
    static void SetMaxLogFiles(int max_files);
    
    // Main logging methods
    static void Log(LogLevel level, const wxString& message);
    static void LogDebug(const wxString& message);
    static void LogInfo(const wxString& message);
    static void LogWarning(const wxString& message);
    static void LogError(const wxString& message);
    static void LogCritical(const wxString& message);
    
    // Performance logging
    static void LogPerformance(const wxString& operation, long long duration_ms);
    static void LogMemoryUsage(const wxString& operation, size_t memory_bytes);
    
    // Conditional logging
    static void LogIf(bool condition, LogLevel level, const wxString& message);
    static void LogDebugIf(bool condition, const wxString& message);
    static void LogInfoIf(bool condition, const wxString& message);
    static void LogWarningIf(bool condition, const wxString& message);
    static void LogErrorIf(bool condition, const wxString& message);
    
    // Formatted logging
    static void LogFormat(LogLevel level, const wxChar* format, ...);
    static void LogDebugFormat(const wxChar* format, ...);
    static void LogInfoFormat(const wxChar* format, ...);
    static void LogWarningFormat(const wxChar* format, ...);
    static void LogErrorFormat(const wxChar* format, ...);
    
    // Specialized logging
    static void LogFunction(const wxString& function_name);
    static void LogFunctionEntry(const wxString& function_name);
    static void LogFunctionExit(const wxString& function_name);
    static void LogSystemInfo();
    static void LogException(const wxString& exception_msg, const wxString& location = wxEmptyString);
    
    // Log management
    static void FlushLogs();
    static void ClearLogFile();
    static void RotateLogFile();
    static wxString GetLogFileName();
    static size_t GetLogFileSize();
    static wxArrayString GetRecentLogEntries(size_t count = 100);
    
    // Utility methods
    static wxString LogLevelToString(LogLevel level);
    static LogLevel StringToLogLevel(const wxString& level_str);
    static wxString GetTimestamp();
    static wxString FormatLogMessage(LogLevel level, const wxString& message);
    
private:
    static LogLevel current_log_level;
    static bool console_output_enabled;
    static bool file_output_enabled;
    static bool timestamp_enabled;
    static wxString log_filename;
    static std::unique_ptr<std::ofstream> log_file_stream;
    static size_t max_log_file_size;
    static int max_log_files;
    static bool initialized;
    
    // Internal methods
    static void WriteToFile(const wxString& formatted_message);
    static void WriteToConsole(LogLevel level, const wxString& formatted_message);
    static void CheckAndRotateLogFile();
    static void CreateLogDirectory();
    static wxString GetDefaultLogFileName();
};

// RAII class for function logging
class FunctionLogger {
public:
    explicit FunctionLogger(const wxString& function_name);
    ~FunctionLogger();
    
private:
    wxString function_name_;
};

// Convenience macros
#ifdef _DEBUG
    #define LOG_FUNCTION() utils::FunctionLogger __func_logger__(__FUNCTION__)
    #define LOG_DEBUG(msg) utils::LogUtils::LogDebug(msg)
    #define LOG_DEBUG_FORMAT(fmt, ...) utils::LogUtils::LogDebugFormat(fmt, __VA_ARGS__)
#else
    #define LOG_FUNCTION() ((void)0)
    #define LOG_DEBUG(msg) ((void)0)
    #define LOG_DEBUG_FORMAT(fmt, ...) ((void)0)
#endif

#define LOG_INFO(msg) utils::LogUtils::LogInfo(msg)
#define LOG_WARNING(msg) utils::LogUtils::LogWarning(msg)
#define LOG_ERROR(msg) utils::LogUtils::LogError(msg)
#define LOG_CRITICAL(msg) utils::LogUtils::LogCritical(msg)

#define LOG_INFO_FORMAT(fmt, ...) utils::LogUtils::LogInfoFormat(fmt, __VA_ARGS__)
#define LOG_WARNING_FORMAT(fmt, ...) utils::LogUtils::LogWarningFormat(fmt, __VA_ARGS__)
#define LOG_ERROR_FORMAT(fmt, ...) utils::LogUtils::LogErrorFormat(fmt, __VA_ARGS__)

}

#endif // __LOG_UTILS_HPP