#ifndef __FILE_UTILS_HPP
#define __FILE_UTILS_HPP

#include <wx/wx.h>
#include <vector>

namespace utils {

// File validation and path utilities
class FileUtils {
public:
    // Supported media extensions
    static const std::vector<wxString> SUPPORTED_VIDEO_EXTENSIONS;
    static const std::vector<wxString> SUPPORTED_AUDIO_EXTENSIONS;
    static const std::vector<wxString> SUPPORTED_PLAYLIST_EXTENSIONS;
    
    // File validation
    static bool IsMediaFile(const wxString& filepath);
    static bool IsVideoFile(const wxString& filepath);
    static bool IsAudioFile(const wxString& filepath);
    static bool IsPlaylistFile(const wxString& filepath);
    static bool FileExists(const wxString& filepath);
    static bool DirectoryExists(const wxString& dirpath);
    
    // Path manipulation
    static wxString GetFileName(const wxString& filepath);
    static wxString GetFileNameWithoutExtension(const wxString& filepath);
    static wxString GetFileExtension(const wxString& filepath);
    static wxString GetFileDirectory(const wxString& filepath);
    static wxString NormalizePath(const wxString& filepath);
    static wxString GetRelativePath(const wxString& filepath, const wxString& base_path);
    static wxString GetAbsolutePath(const wxString& filepath);
    
    // File size utilities
    static wxString FormatFileSize(wxULongLong bytes);
    static wxULongLong GetFileSize(const wxString& filepath);
    static wxDateTime GetFileModificationTime(const wxString& filepath);
    static wxDateTime GetFileCreationTime(const wxString& filepath);
    
    // Directory operations
    static wxArrayString GetFilesInDirectory(const wxString& directory, 
                                           bool recursive = false,
                                           const wxString& pattern = "*");
    static wxArrayString GetMediaFilesInDirectory(const wxString& directory, 
                                                bool recursive = false);
    static bool CreateDirectory(const wxString& directory);
    static bool DeleteFile(const wxString& filepath);
    static bool CopyFile(const wxString& source, const wxString& destination);
    static bool MoveFile(const wxString& source, const wxString& destination);
    
    // File content utilities
    static wxString ReadTextFile(const wxString& filepath);
    static bool WriteTextFile(const wxString& filepath, const wxString& content);
    static wxArrayString ReadLinesFromFile(const wxString& filepath);
    static bool WritLinesToFile(const wxString& filepath, const wxArrayString& lines);
    
    // Media file utilities
    static bool IsValidMediaFormat(const wxString& filepath);
    static wxString GetMimeType(const wxString& filepath);
    static bool CanPlayFile(const wxString& filepath);
    
    // File backup utilities
    static wxString CreateBackupFilename(const wxString& filepath);
    static bool BackupFile(const wxString& filepath);
    static bool RestoreBackup(const wxString& filepath);
    
    // Temporary file utilities
    static wxString GetTempDirectory();
    static wxString CreateTempFile(const wxString& prefix = "wanjplayer");
    static bool CleanupTempFiles();
    
    // File comparison
    static bool AreFilesEqual(const wxString& file1, const wxString& file2);
    static wxString GetFileChecksum(const wxString& filepath);
    
private:
    // Helper methods
    static bool HasExtension(const wxString& filepath, const std::vector<wxString>& extensions);
    static wxString NormalizeExtension(const wxString& extension);
};

}

#endif // __FILE_UTILS_HPP