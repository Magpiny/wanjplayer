#ifndef __PERFORMANCE_UTILS_HPP
#define __PERFORMANCE_UTILS_HPP

#include <wx/wx.h>
#include <chrono>
#include <vector>
#include <memory>
#include <map>

namespace utils {

// Performance and memory utilities
class PerformanceUtils {
public:
    // Memory management
    static void ClearUnusedMemory();
    static size_t GetCurrentMemoryUsage();
    static size_t GetPeakMemoryUsage();
    static void ResetPeakMemoryUsage();
    static wxString FormatMemorySize(size_t bytes);
    
    // String optimization
    static wxString OptimizeString(const wxString& str);
    static void OptimizeStringArray(wxArrayString& strings);
    
    // Collection utilities
    template<typename T>
    static void OptimizeVector(std::vector<T>& vec) {
        if (vec.capacity() > vec.size() * 2) {
            std::vector<T> temp(vec);
            vec.swap(temp);
        }
    }
    
    template<typename T>
    static void ShrinkToFit(std::vector<T>& vec) {
        vec.shrink_to_fit();
    }
    
    // Timing utilities for performance measurement
    static std::chrono::high_resolution_clock::time_point StartTimer();
    static long long EndTimer(const std::chrono::high_resolution_clock::time_point& start);
    static long long GetElapsedMilliseconds(const std::chrono::high_resolution_clock::time_point& start);
    static long long GetElapsedMicroseconds(const std::chrono::high_resolution_clock::time_point& start);
    
    // Performance monitoring
    static void StartPerformanceMonitoring();
    static void StopPerformanceMonitoring();
    static bool IsPerformanceMonitoringActive();
    static void RecordOperation(const wxString& operation_name, long long duration_ms);
    static void RecordMemoryOperation(const wxString& operation_name, size_t memory_bytes);
    
    // Performance statistics
    static long long GetAverageOperationTime(const wxString& operation_name);
    static long long GetMaxOperationTime(const wxString& operation_name);
    static long long GetMinOperationTime(const wxString& operation_name);
    static size_t GetOperationCount(const wxString& operation_name);
    static wxString GetPerformanceReport();
    static void ClearPerformanceData();
    
    // System information
    static size_t GetAvailableMemory();
    static size_t GetTotalMemory();
    static double GetMemoryUsagePercentage();
    static int GetCPUUsagePercentage();
    static wxString GetSystemInfo();
    
    // Cache management
    static void ClearAllCaches();
    static size_t GetCacheMemoryUsage();
    static void SetMaxCacheSize(size_t max_size_bytes);
    static size_t GetMaxCacheSize();
    
    // Profiling utilities
    static void EnableProfiling(bool enable);
    static bool IsProfilingEnabled();
    static void SaveProfilingData(const wxString& filename);
    static void LoadProfilingData(const wxString& filename);
    
private:
    // Performance data structures
    struct OperationStats {
        long long total_time;
        long long min_time;
        long long max_time;
        size_t count;
        size_t total_memory;
        
        OperationStats() : total_time(0), min_time(LLONG_MAX), max_time(0), count(0), total_memory(0) {}
    };
    
    static std::map<wxString, OperationStats> operation_stats;
    static bool monitoring_active;
    static bool profiling_enabled;
    static size_t peak_memory_usage;
    static size_t max_cache_size;
    static std::chrono::high_resolution_clock::time_point monitoring_start_time;
    
    // Helper methods
    static void UpdateOperationStats(const wxString& operation_name, long long duration_ms, size_t memory_bytes = 0);
    static size_t GetProcessMemoryUsage();
    static void CollectGarbage();
};

// RAII class for automatic performance timing
class PerformanceTimer {
public:
    explicit PerformanceTimer(const wxString& operation_name, bool auto_record = true);
    ~PerformanceTimer();
    
    long long GetElapsedTime() const;
    void Record();
    void Reset();
    
private:
    wxString operation_name_;
    std::chrono::high_resolution_clock::time_point start_time_;
    bool auto_record_;
    bool recorded_;
};

// Memory usage tracker
class MemoryTracker {
public:
    explicit MemoryTracker(const wxString& operation_name);
    ~MemoryTracker();
    
    void UpdateMemoryUsage();
    size_t GetMemoryDelta() const;
    
private:
    wxString operation_name_;
    size_t start_memory_;
    size_t peak_memory_;
};

// Convenience macros
#ifdef _DEBUG
    #define PERFORMANCE_TIMER(name) utils::PerformanceTimer __perf_timer__(name)
    #define MEMORY_TRACKER(name) utils::MemoryTracker __mem_tracker__(name)
#else
    #define PERFORMANCE_TIMER(name) ((void)0)
    #define MEMORY_TRACKER(name) ((void)0)
#endif

#define PERFORMANCE_TIMER_ALWAYS(name) utils::PerformanceTimer __perf_timer__(name)
#define MEMORY_TRACKER_ALWAYS(name) utils::MemoryTracker __mem_tracker__(name)

}

#endif // __PERFORMANCE_UTILS_HPP