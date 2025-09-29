#include "log_utils.hpp"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/datetime.h>
#include <wx/textfile.h>
#include <iostream>

namespace utils {

// Static member definitions
LogUtils::LogLevel LogUtils::current_log_level = LogLevel::INFO;
bool LogUtils::console_output_enabled = true;
bool LogUtils::file_output_enabled = false;
bool LogUtils::timestamp_enabled = true;
wxString LogUtils::log_filename;
std::unique_ptr<std::ofstream> LogUtils::log_file_stream;
size_t LogUtils::max_log_file_size = 10 * 1024 * 1024; // 10MB
int LogUtils::max_log_files = 5;
bool LogUtils::initialized = false;

void LogUtils::Initialize(const wxString& log_file)
{
    if (initialized) {
        return;
    }
    
    if (!log_file.IsEmpty()) {
        log_filename = log_file;
        EnableFileOutput(true, log_file);
    } else {
        log_filename = GetDefaultLogFileName();
    }
    
    initialized = true;
    LogInfo("LogUtils initialized");
}

void LogUtils::Shutdown()
{
    if (log_file_stream && log_file_stream->is_open()) {
        log_file_stream->close();
    }
    log_file_stream.reset();
    initialized = false;
}

void LogUtils::SetLogLevel(LogLevel level)
{
    current_log_level = level;
}

LogUtils::LogLevel LogUtils::GetLogLevel()
{
    return current_log_level;
}

void LogUtils::EnableConsoleOutput(bool enable)
{
    console_output_enabled = enable;
}

void LogUtils::EnableFileOutput(bool enable, const wxString& filename)
{
    file_output_enabled = enable;
    
    if (enable) {
        if (!filename.IsEmpty()) {
            log_filename = filename;
        }
        
        CreateLogDirectory();
        
        log_file_stream = std::make_unique<std::ofstream>(log_filename.ToStdString(), std::ios::app);
        if (!log_file_stream->is_open()) {
            console_output_enabled = true; // Fallback to console
            LogError("Failed to open log file: " + log_filename);
        }
    } else {
        if (log_file_stream && log_file_stream->is_open()) {
            log_file_stream->close();
        }
        log_file_stream.reset();
    }
}

void LogUtils::EnableTimestamp(bool enable)
{
    timestamp_enabled = enable;
}

void LogUtils::SetMaxLogFileSize(size_t max_size_bytes)
{
    max_log_file_size = max_size_bytes;
}

void LogUtils::SetMaxLogFiles(int max_files)
{
    max_log_files = max_files;
}

void LogUtils::Log(LogLevel level, const wxString& message)
{
    if (level < current_log_level) {
        return;
    }
    
    wxString formatted_message = FormatLogMessage(level, message);
    
    if (console_output_enabled) {
        WriteToConsole(level, formatted_message);
    }
    
    if (file_output_enabled) {
        WriteToFile(formatted_message);
        CheckAndRotateLogFile();
    }
}

void LogUtils::LogDebug(const wxString& message)
{
    Log(LogLevel::DEBUG, message);
}

void LogUtils::LogInfo(const wxString& message)
{
    Log(LogLevel::INFO, message);
}

void LogUtils::LogWarning(const wxString& message)
{
    Log(LogLevel::WARNING, message);
}

void LogUtils::LogError(const wxString& message)
{
    Log(LogLevel::ERROR, message);
}

void LogUtils::LogCritical(const wxString& message)
{
    Log(LogLevel::CRITICAL, message);
}

void LogUtils::LogPerformance(const wxString& operation, long long duration_ms)
{
    LogInfo(wxString::Format("[PERFORMANCE] %s: %lld ms", operation, duration_ms));
}

void LogUtils::LogMemoryUsage(const wxString& operation, size_t memory_bytes)
{
    LogInfo(wxString::Format("[MEMORY] %s: %zu bytes", operation, memory_bytes));
}

void LogUtils::LogIf(bool condition, LogLevel level, const wxString& message)
{
    if (condition) {
        Log(level, message);
    }
}

void LogUtils::LogDebugIf(bool condition, const wxString& message)
{
    LogIf(condition, LogLevel::DEBUG, message);
}

void LogUtils::LogInfoIf(bool condition, const wxString& message)
{
    LogIf(condition, LogLevel::INFO, message);
}

void LogUtils::LogWarningIf(bool condition, const wxString& message)
{
    LogIf(condition, LogLevel::WARNING, message);
}

void LogUtils::LogErrorIf(bool condition, const wxString& message)
{
    LogIf(condition, LogLevel::ERROR, message);
}

void LogUtils::LogFormat(LogLevel level, const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString message = wxString::FormatV(format, args);
    va_end(args);
    Log(level, message);
}

void LogUtils::LogDebugFormat(const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString message = wxString::FormatV(format, args);
    va_end(args);
    LogDebug(message);
}

void LogUtils::LogInfoFormat(const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString message = wxString::FormatV(format, args);
    va_end(args);
    LogInfo(message);
}

void LogUtils::LogWarningFormat(const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString message = wxString::FormatV(format, args);
    va_end(args);
    LogWarning(message);
}

void LogUtils::LogErrorFormat(const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString message = wxString::FormatV(format, args);
    va_end(args);
    LogError(message);
}

void LogUtils::LogFunction(const wxString& function_name)
{
    LogDebug("Function: " + function_name);
}

void LogUtils::LogFunctionEntry(const wxString& function_name)
{
    LogDebug("Entering: " + function_name);
}

void LogUtils::LogFunctionExit(const wxString& function_name)
{
    LogDebug("Exiting: " + function_name);
}

void LogUtils::LogSystemInfo()
{
    LogInfo("System: " + wxGetOsDescription());
    LogInfo("wxWidgets: " + wxGetLibraryVersionInfo().GetVersionString());
}

void LogUtils::LogException(const wxString& exception_msg, const wxString& location)
{
    wxString full_message = "Exception: " + exception_msg;
    if (!location.IsEmpty()) {
        full_message += " at " + location;
    }
    LogError(full_message);
}

void LogUtils::FlushLogs()
{
    if (log_file_stream && log_file_stream->is_open()) {
        log_file_stream->flush();
    }
}

void LogUtils::ClearLogFile()
{
    if (log_file_stream && log_file_stream->is_open()) {
        log_file_stream->close();
    }
    
    if (wxFileExists(log_filename)) {
        wxRemoveFile(log_filename);
    }
    
    if (file_output_enabled) {
        log_file_stream = std::make_unique<std::ofstream>(log_filename.ToStdString());
    }
}

void LogUtils::RotateLogFile()
{
    if (!file_output_enabled || log_filename.IsEmpty()) {
        return;
    }
    
    // Close current log file
    if (log_file_stream && log_file_stream->is_open()) {
        log_file_stream->close();
    }
    
    // Rotate existing log files
    for (int i = max_log_files - 1; i > 0; i--) {
        wxString old_name = wxString::Format("%s.%d", log_filename, i);
        wxString new_name = wxString::Format("%s.%d", log_filename, i + 1);
        
        if (wxFileExists(old_name)) {
            if (i == max_log_files - 1) {
                wxRemoveFile(old_name); // Remove oldest file
            } else {
                wxRenameFile(old_name, new_name);
            }
        }
    }
    
    // Rename current log to .1
    if (wxFileExists(log_filename)) {
        wxString backup_name = log_filename + ".1";
        wxRenameFile(log_filename, backup_name);
    }
    
    // Create new log file
    log_file_stream = std::make_unique<std::ofstream>(log_filename.ToStdString());
}

wxString LogUtils::GetLogFileName()
{
    return log_filename;
}

size_t LogUtils::GetLogFileSize()
{
    if (!wxFileExists(log_filename)) {
        return 0;
    }
    
    wxFileName fn(log_filename);
    return fn.GetSize().GetValue();
}

wxArrayString LogUtils::GetRecentLogEntries(size_t count)
{
    wxArrayString entries;
    
    if (!wxFileExists(log_filename)) {
        return entries;
    }
    
    wxTextFile file(log_filename);
    if (file.Open()) {
        size_t start = 0;
        if (file.GetLineCount() > count) {
            start = file.GetLineCount() - count;
        }
        
        for (size_t i = start; i < file.GetLineCount(); i++) {
            entries.Add(file.GetLine(i));
        }
        file.Close();
    }
    
    return entries;
}

wxString LogUtils::LogLevelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

LogUtils::LogLevel LogUtils::StringToLogLevel(const wxString& level_str)
{
    wxString upper = level_str.Upper();
    if (upper == "DEBUG") return LogLevel::DEBUG;
    if (upper == "INFO") return LogLevel::INFO;
    if (upper == "WARNING") return LogLevel::WARNING;
    if (upper == "ERROR") return LogLevel::ERROR;
    if (upper == "CRITICAL") return LogLevel::CRITICAL;
    return LogLevel::INFO; // Default
}

wxString LogUtils::GetTimestamp()
{
    return wxDateTime::Now().Format("%Y-%m-%d %H:%M:%S");
}

wxString LogUtils::FormatLogMessage(LogLevel level, const wxString& message)
{
    wxString formatted;
    
    if (timestamp_enabled) {
        formatted += "[" + GetTimestamp() + "] ";
    }
    
    formatted += "[" + LogLevelToString(level) + "] ";
    formatted += message;
    
    return formatted;
}

void LogUtils::WriteToFile(const wxString& formatted_message)
{
    if (log_file_stream && log_file_stream->is_open()) {
        *log_file_stream << formatted_message.ToStdString() << std::endl;
    }
}

void LogUtils::WriteToConsole(LogLevel level, const wxString& formatted_message)
{
    if (level >= LogLevel::ERROR) {
        std::cerr << formatted_message.ToStdString() << std::endl;
    } else {
        std::cout << formatted_message.ToStdString() << std::endl;
    }
    
    // Also use wxWidgets logging
    wxLogMessage(formatted_message);
}

void LogUtils::CheckAndRotateLogFile()
{
    if (GetLogFileSize() > max_log_file_size) {
        RotateLogFile();
    }
}

void LogUtils::CreateLogDirectory()
{
    if (log_filename.IsEmpty()) {
        return;
    }
    
    wxFileName fn(log_filename);
    wxString dir = fn.GetPath();
    
    if (!dir.IsEmpty() && !wxDirExists(dir)) {
        wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }
}

wxString LogUtils::GetDefaultLogFileName()
{
    wxStandardPaths& paths = wxStandardPaths::Get();
    wxString log_dir = paths.GetUserDataDir();
    
    if (!wxDirExists(log_dir)) {
        wxFileName::Mkdir(log_dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }
    
    return wxFileName(log_dir, "wanjplayer.log").GetFullPath();
}

// FunctionLogger implementation
FunctionLogger::FunctionLogger(const wxString& function_name)
    : function_name_(function_name)
{
    LogUtils::LogFunctionEntry(function_name_);
}

FunctionLogger::~FunctionLogger()
{
    LogUtils::LogFunctionExit(function_name_);
}

}