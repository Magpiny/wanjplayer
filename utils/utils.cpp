#include "utils.hpp"

namespace utils {

// Static member definitions
bool UtilsManager::initialized = false;

void UtilsManager::Initialize()
{
    if (initialized) {
        return;
    }
    
    // Initialize logging system
    LogUtils::Initialize();
    LogUtils::SetLogLevel(LogUtils::LogLevel::INFO);
    LogUtils::EnableConsoleOutput(true);
    
    // Initialize performance monitoring
    PerformanceUtils::StartPerformanceMonitoring();
    
    // Set default configurations
    PerformanceUtils::SetMaxCacheSize(100 * 1024 * 1024); // 100MB
    
    initialized = true;
    LogUtils::LogInfo("UtilsManager initialized successfully");
}

void UtilsManager::Cleanup()
{
    if (!initialized) {
        return;
    }
    
    LogUtils::LogInfo("UtilsManager shutting down");
    
    // Stop performance monitoring
    PerformanceUtils::StopPerformanceMonitoring();
    
    // Clean up temporary files
    FileUtils::CleanupTempFiles();
    
    // Clear caches
    PerformanceUtils::ClearAllCaches();
    
    // Shutdown logging system
    LogUtils::Shutdown();
    
    initialized = false;
}

bool UtilsManager::IsInitialized()
{
    return initialized;
}

void UtilsManager::SetLogLevel(LogUtils::LogLevel level)
{
    LogUtils::SetLogLevel(level);
}

void UtilsManager::EnablePerformanceLogging(bool enable)
{
    PerformanceUtils::EnableProfiling(enable);
}

void UtilsManager::SetTempDirectory(const wxString& directory)
{
    // This would set the temporary directory if we had such functionality
    // For now, we'll just log it
    LogUtils::LogInfo("Temp directory set to: " + directory);
}

}