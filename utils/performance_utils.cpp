#include "performance_utils.hpp"
#include "log_utils.hpp"
#include <wx/utils.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <map>
#include <algorithm>
#include <climits>

#ifdef __WXMSW__
#include <windows.h>
#include <psapi.h>
#elif __WXGTK__
#include <unistd.h>
#include <fstream>
#include <sstream>
#elif __WXMAC__
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

namespace utils {

// Static member definitions
std::map<wxString, PerformanceUtils::OperationStats> PerformanceUtils::operation_stats;
bool PerformanceUtils::monitoring_active = false;
bool PerformanceUtils::profiling_enabled = false;
size_t PerformanceUtils::peak_memory_usage = 0;
size_t PerformanceUtils::max_cache_size = 100 * 1024 * 1024; // 100MB default
std::chrono::high_resolution_clock::time_point PerformanceUtils::monitoring_start_time;

void PerformanceUtils::ClearUnusedMemory()
{
#ifdef __WXDEBUG__
    wxLog::FlushActive();
#endif
    
    // Force garbage collection if available
    // This is a basic implementation - could be enhanced
    static size_t gc_counter = 0;
    gc_counter++;
    
    if (gc_counter % 100 == 0) {
        CollectGarbage();
    }
}

size_t PerformanceUtils::GetCurrentMemoryUsage()
{
    return GetProcessMemoryUsage();
}

size_t PerformanceUtils::GetPeakMemoryUsage()
{
    return peak_memory_usage;
}

void PerformanceUtils::ResetPeakMemoryUsage()
{
    peak_memory_usage = GetCurrentMemoryUsage();
}

wxString PerformanceUtils::FormatMemorySize(size_t bytes)
{
    const size_t KB = 1024;
    const size_t MB = KB * 1024;
    const size_t GB = MB * 1024;

    if (bytes < KB) {
        return wxString::Format("%zu B", bytes);
    } else if (bytes < MB) {
        return wxString::Format("%.1f KB", (double)bytes / KB);
    } else if (bytes < GB) {
        return wxString::Format("%.1f MB", (double)bytes / MB);
    } else {
        return wxString::Format("%.1f GB", (double)bytes / GB);
    }
}

wxString PerformanceUtils::OptimizeString(const wxString& str)
{
    wxString optimized = str;
    optimized.Trim(true);
    optimized.Trim(false);
    return optimized;
}

void PerformanceUtils::OptimizeStringArray(wxArrayString& strings)
{
    for (size_t i = 0; i < strings.GetCount(); i++) {
        strings[i] = OptimizeString(strings[i]);
    }
}

std::chrono::high_resolution_clock::time_point PerformanceUtils::StartTimer()
{
    return std::chrono::high_resolution_clock::now();
}

long long PerformanceUtils::EndTimer(const std::chrono::high_resolution_clock::time_point& start)
{
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

long long PerformanceUtils::GetElapsedMilliseconds(const std::chrono::high_resolution_clock::time_point& start)
{
    return EndTimer(start);
}

long long PerformanceUtils::GetElapsedMicroseconds(const std::chrono::high_resolution_clock::time_point& start)
{
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void PerformanceUtils::StartPerformanceMonitoring()
{
    monitoring_active = true;
    monitoring_start_time = std::chrono::high_resolution_clock::now();
    operation_stats.clear();
    ResetPeakMemoryUsage();
    
    LogUtils::LogInfo("Performance monitoring started");
}

void PerformanceUtils::StopPerformanceMonitoring()
{
    monitoring_active = false;
    LogUtils::LogInfo("Performance monitoring stopped");
}

bool PerformanceUtils::IsPerformanceMonitoringActive()
{
    return monitoring_active;
}

void PerformanceUtils::RecordOperation(const wxString& operation_name, long long duration_ms)
{
    if (!monitoring_active) {
        return;
    }
    
    UpdateOperationStats(operation_name, duration_ms, 0);
    
    if (profiling_enabled) {
        LogUtils::LogPerformance(operation_name, duration_ms);
    }
}

void PerformanceUtils::RecordMemoryOperation(const wxString& operation_name, size_t memory_bytes)
{
    if (!monitoring_active) {
        return;
    }
    
    UpdateOperationStats(operation_name, 0, memory_bytes);
    
    size_t current_memory = GetCurrentMemoryUsage();
    if (current_memory > peak_memory_usage) {
        peak_memory_usage = current_memory;
    }
    
    if (profiling_enabled) {
        LogUtils::LogMemoryUsage(operation_name, memory_bytes);
    }
}

long long PerformanceUtils::GetAverageOperationTime(const wxString& operation_name)
{
    auto it = operation_stats.find(operation_name);
    if (it == operation_stats.end() || it->second.count == 0) {
        return 0;
    }
    
    return it->second.total_time / it->second.count;
}

long long PerformanceUtils::GetMaxOperationTime(const wxString& operation_name)
{
    auto it = operation_stats.find(operation_name);
    if (it == operation_stats.end()) {
        return 0;
    }
    
    return it->second.max_time;
}

long long PerformanceUtils::GetMinOperationTime(const wxString& operation_name)
{
    auto it = operation_stats.find(operation_name);
    if (it == operation_stats.end()) {
        return 0;
    }
    
    return it->second.min_time == LLONG_MAX ? 0 : it->second.min_time;
}

size_t PerformanceUtils::GetOperationCount(const wxString& operation_name)
{
    auto it = operation_stats.find(operation_name);
    if (it == operation_stats.end()) {
        return 0;
    }
    
    return it->second.count;
}

wxString PerformanceUtils::GetPerformanceReport()
{
    wxString report = "Performance Report:\n";
    report += "==================\n\n";
    
    if (!monitoring_active && operation_stats.empty()) {
        report += "No performance data available.\n";
        return report;
    }
    
    report += wxString::Format("Peak Memory Usage: %s\n", FormatMemorySize(peak_memory_usage));
    report += wxString::Format("Current Memory Usage: %s\n\n", FormatMemorySize(GetCurrentMemoryUsage()));
    
    if (!operation_stats.empty()) {
        report += "Operation Statistics:\n";
        report += "--------------------\n";
        
        for (const auto& pair : operation_stats) {
            const wxString& op_name = pair.first;
            const OperationStats& stats = pair.second;
            
            report += wxString::Format("Operation: %s\n", op_name);
            report += wxString::Format("  Count: %zu\n", stats.count);
            if (stats.total_time > 0) {
                report += wxString::Format("  Average Time: %lld ms\n", stats.total_time / stats.count);
                report += wxString::Format("  Min Time: %lld ms\n", stats.min_time == LLONG_MAX ? 0 : stats.min_time);
                report += wxString::Format("  Max Time: %lld ms\n", stats.max_time);
            }
            if (stats.total_memory > 0) {
                report += wxString::Format("  Total Memory: %s\n", FormatMemorySize(stats.total_memory));
            }
            report += "\n";
        }
    }
    
    return report;
}

void PerformanceUtils::ClearPerformanceData()
{
    operation_stats.clear();
    ResetPeakMemoryUsage();
    LogUtils::LogInfo("Performance data cleared");
}

size_t PerformanceUtils::GetAvailableMemory()
{
#ifdef __WXMSW__
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return static_cast<size_t>(memInfo.ullAvailPhys);
#elif __WXGTK__
    long pages = sysconf(_SC_AVPHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return static_cast<size_t>(pages * page_size);
#else
    return 0; // Not implemented for other platforms
#endif
}

size_t PerformanceUtils::GetTotalMemory()
{
#ifdef __WXMSW__
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return static_cast<size_t>(memInfo.ullTotalPhys);
#elif __WXGTK__
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return static_cast<size_t>(pages * page_size);
#else
    return 0; // Not implemented for other platforms
#endif
}

double PerformanceUtils::GetMemoryUsagePercentage()
{
    size_t total = GetTotalMemory();
    if (total == 0) {
        return 0.0;
    }
    
    size_t used = GetCurrentMemoryUsage();
    return (double)used / total * 100.0;
}

int PerformanceUtils::GetCPUUsagePercentage()
{
    // Simple implementation - could be enhanced
    return 0; // Not implemented
}

wxString PerformanceUtils::GetSystemInfo()
{
    wxString info;
    info += "System Information:\n";
    info += "==================\n";
    info += wxString::Format("OS: %s\n", wxGetOsDescription());
    info += wxString::Format("Total Memory: %s\n", FormatMemorySize(GetTotalMemory()));
    info += wxString::Format("Available Memory: %s\n", FormatMemorySize(GetAvailableMemory()));
    info += wxString::Format("Memory Usage: %.1f%%\n", GetMemoryUsagePercentage());
    
    return info;
}

void PerformanceUtils::ClearAllCaches()
{
    // Clear various caches - this is application-specific
    LogUtils::LogInfo("Caches cleared");
}

size_t PerformanceUtils::GetCacheMemoryUsage()
{
    // Return estimated cache memory usage
    return 0; // Not implemented
}

void PerformanceUtils::SetMaxCacheSize(size_t max_size_bytes)
{
    max_cache_size = max_size_bytes;
}

size_t PerformanceUtils::GetMaxCacheSize()
{
    return max_cache_size;
}

void PerformanceUtils::EnableProfiling(bool enable)
{
    profiling_enabled = enable;
    LogUtils::LogInfo(wxString::Format("Profiling %s", enable ? "enabled" : "disabled"));
}

bool PerformanceUtils::IsProfilingEnabled()
{
    return profiling_enabled;
}

void PerformanceUtils::SaveProfilingData(const wxString& filename)
{
    wxString report = GetPerformanceReport();
    
    wxTextFile file;
    if (wxFileExists(filename)) {
        file.Open(filename);
        file.Clear();
    } else {
        file.Create(filename);
    }
    
    wxStringTokenizer tokenizer(report, "\n");
    while (tokenizer.HasMoreTokens()) {
        file.AddLine(tokenizer.GetNextToken());
    }
    
    file.Write();
    file.Close();
    
    LogUtils::LogInfo("Profiling data saved to: " + filename);
}

void PerformanceUtils::LoadProfilingData(const wxString& filename)
{
    // Not implemented - would load previous profiling data
    LogUtils::LogInfo("Load profiling data not implemented");
}

void PerformanceUtils::UpdateOperationStats(const wxString& operation_name, long long duration_ms, size_t memory_bytes)
{
    OperationStats& stats = operation_stats[operation_name];
    
    if (duration_ms > 0) {
        stats.total_time += duration_ms;
        stats.min_time = std::min(stats.min_time, duration_ms);
        stats.max_time = std::max(stats.max_time, duration_ms);
    }
    
    if (memory_bytes > 0) {
        stats.total_memory += memory_bytes;
    }
    
    stats.count++;
}

size_t PerformanceUtils::GetProcessMemoryUsage()
{
#ifdef __WXMSW__
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return static_cast<size_t>(pmc.WorkingSetSize);
    }
#elif __WXGTK__
    std::ifstream statm("/proc/self/statm");
    if (statm.is_open()) {
        size_t size, resident;
        statm >> size >> resident;
        statm.close();
        long page_size = sysconf(_SC_PAGE_SIZE);
        return resident * page_size;
    }
#elif __WXMAC__
    struct mach_task_basic_info info;
    mach_msg_type_number_t info_count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  (task_info_t)&info, &info_count) == KERN_SUCCESS) {
        return static_cast<size_t>(info.resident_size);
    }
#endif
    return 0;
}

void PerformanceUtils::CollectGarbage()
{
    // Basic garbage collection simulation
    ClearUnusedMemory();
}

// PerformanceTimer implementation

PerformanceTimer::PerformanceTimer(const wxString& operation_name, bool auto_record)
    : operation_name_(operation_name)
    , auto_record_(auto_record)
    , recorded_(false)
{
    Reset();
}

PerformanceTimer::~PerformanceTimer()
{
    if (auto_record_ && !recorded_) {
        Record();
    }
}

long long PerformanceTimer::GetElapsedTime() const
{
    return PerformanceUtils::EndTimer(start_time_);
}

void PerformanceTimer::Record()
{
    if (!recorded_) {
        long long elapsed = GetElapsedTime();
        PerformanceUtils::RecordOperation(operation_name_, elapsed);
        recorded_ = true;
    }
}

void PerformanceTimer::Reset()
{
    start_time_ = PerformanceUtils::StartTimer();
    recorded_ = false;
}

// MemoryTracker implementation

MemoryTracker::MemoryTracker(const wxString& operation_name)
    : operation_name_(operation_name)
    , start_memory_(PerformanceUtils::GetCurrentMemoryUsage())
    , peak_memory_(start_memory_)
{
}

MemoryTracker::~MemoryTracker()
{
    size_t end_memory = PerformanceUtils::GetCurrentMemoryUsage();
    size_t delta = (end_memory > start_memory_) ? (end_memory - start_memory_) : 0;
    
    if (delta > 0) {
        PerformanceUtils::RecordMemoryOperation(operation_name_, delta);
    }
}

void MemoryTracker::UpdateMemoryUsage()
{
    size_t current = PerformanceUtils::GetCurrentMemoryUsage();
    if (current > peak_memory_) {
        peak_memory_ = current;
    }
}

size_t MemoryTracker::GetMemoryDelta() const
{
    size_t current = PerformanceUtils::GetCurrentMemoryUsage();
    return (current > start_memory_) ? (current - start_memory_) : 0;
}

}