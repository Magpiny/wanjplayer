#include "file_utils.hpp"
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/mimetype.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/filefn.h>
#include <wx/tokenzr.h>
#include <algorithm>
#include <fstream>

namespace utils {

// Static member definitions
const std::vector<wxString> FileUtils::SUPPORTED_VIDEO_EXTENSIONS = {
    "mp4", "avi", "mkv", "mov", "wmv", "flv", "webm", "m4v", "3gp", "ogv",
    "mpg", "mpeg", "m2v", "vob", "ts", "mts", "m2ts", "divx", "xvid", "asf"
};

const std::vector<wxString> FileUtils::SUPPORTED_AUDIO_EXTENSIONS = {
    "mp3", "wav", "flac", "aac", "ogg", "wma", "m4a", "opus", "aiff", "au",
    "ra", "amr", "3ga", "ac3", "dts", "ape", "mka", "oga", "spx", "tta"
};

const std::vector<wxString> FileUtils::SUPPORTED_PLAYLIST_EXTENSIONS = {
    "m3u", "m3u8", "pls", "xspf", "wpl", "asx", "b4s", "kpl"
};

// File validation
bool FileUtils::IsMediaFile(const wxString& filepath)
{
    return IsVideoFile(filepath) || IsAudioFile(filepath);
}

bool FileUtils::IsVideoFile(const wxString& filepath)
{
    return HasExtension(filepath, SUPPORTED_VIDEO_EXTENSIONS);
}

bool FileUtils::IsAudioFile(const wxString& filepath)
{
    return HasExtension(filepath, SUPPORTED_AUDIO_EXTENSIONS);
}

bool FileUtils::IsPlaylistFile(const wxString& filepath)
{
    return HasExtension(filepath, SUPPORTED_PLAYLIST_EXTENSIONS);
}

bool FileUtils::FileExists(const wxString& filepath)
{
    return wxFileName::FileExists(filepath);
}

bool FileUtils::DirectoryExists(const wxString& dirpath)
{
    return wxFileName::DirExists(dirpath);
}

// Path manipulation
wxString FileUtils::GetFileName(const wxString& filepath)
{
    wxFileName fn(filepath);
    return fn.GetFullName();
}

wxString FileUtils::GetFileNameWithoutExtension(const wxString& filepath)
{
    wxFileName fn(filepath);
    return fn.GetName();
}

wxString FileUtils::GetFileExtension(const wxString& filepath)
{
    wxFileName fn(filepath);
    return fn.GetExt().Lower();
}

wxString FileUtils::GetFileDirectory(const wxString& filepath)
{
    wxFileName fn(filepath);
    return fn.GetPath();
}

wxString FileUtils::NormalizePath(const wxString& filepath)
{
    wxFileName fn(filepath);
    fn.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE | wxPATH_NORM_LONG | wxPATH_NORM_CASE);
    return fn.GetFullPath();
}

wxString FileUtils::GetRelativePath(const wxString& filepath, const wxString& base_path)
{
    wxFileName fn(filepath);
    wxFileName base(base_path);
    fn.MakeRelativeTo(base.GetPath());
    return fn.GetFullPath();
}

wxString FileUtils::GetAbsolutePath(const wxString& filepath)
{
    wxFileName fn(filepath);
    fn.MakeAbsolute();
    return fn.GetFullPath();
}

// File size utilities
wxString FileUtils::FormatFileSize(wxULongLong bytes)
{
    const wxULongLong KB = 1024;
    const wxULongLong MB = KB * 1024;
    const wxULongLong GB = MB * 1024;
    const wxULongLong TB = GB * 1024;

    if (bytes < KB) {
        return wxString::Format("%llu B", bytes.GetValue());
    } else if (bytes < MB) {
        return wxString::Format("%.1f KB", bytes.ToDouble() / KB.ToDouble());
    } else if (bytes < GB) {
        return wxString::Format("%.1f MB", bytes.ToDouble() / MB.ToDouble());
    } else if (bytes < TB) {
        return wxString::Format("%.1f GB", bytes.ToDouble() / GB.ToDouble());
    } else {
        return wxString::Format("%.1f TB", bytes.ToDouble() / TB.ToDouble());
    }
}

wxULongLong FileUtils::GetFileSize(const wxString& filepath)
{
    wxFileName fn(filepath);
    return fn.GetSize();
}

wxDateTime FileUtils::GetFileModificationTime(const wxString& filepath)
{
    wxFileName fn(filepath);
    wxDateTime dt;
    fn.GetTimes(nullptr, &dt, nullptr);
    return dt;
}

wxDateTime FileUtils::GetFileCreationTime(const wxString& filepath)
{
    wxFileName fn(filepath);
    wxDateTime dt;
    fn.GetTimes(nullptr, nullptr, &dt);
    return dt;
}

// Directory operations
wxArrayString FileUtils::GetFilesInDirectory(const wxString& directory, bool recursive, const wxString& pattern)
{
    wxArrayString files;
    
    if (!DirectoryExists(directory)) {
        return files;
    }
    
    if (recursive) {
        wxDir::GetAllFiles(directory, &files, pattern, wxDIR_FILES);
    } else {
        wxDir dir(directory);
        if (dir.IsOpened()) {
            wxString filename;
            bool cont = dir.GetFirst(&filename, pattern, wxDIR_FILES);
            while (cont) {
                files.Add(wxFileName(directory, filename).GetFullPath());
                cont = dir.GetNext(&filename);
            }
        }
    }
    
    return files;
}

wxArrayString FileUtils::GetMediaFilesInDirectory(const wxString& directory, bool recursive)
{
    wxArrayString all_files = GetFilesInDirectory(directory, recursive);
    wxArrayString media_files;
    
    for (const wxString& file : all_files) {
        if (IsMediaFile(file)) {
            media_files.Add(file);
        }
    }
    
    return media_files;
}

bool FileUtils::CreateDirectory(const wxString& directory)
{
    wxFileName fn;
    fn.AssignDir(directory);
    return fn.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
}

bool FileUtils::DeleteFile(const wxString& filepath)
{
    return wxRemoveFile(filepath);
}

bool FileUtils::CopyFile(const wxString& source, const wxString& destination)
{
    return wxCopyFile(source, destination);
}

bool FileUtils::MoveFile(const wxString& source, const wxString& destination)
{
    return wxRenameFile(source, destination);
}

// File content utilities
wxString FileUtils::ReadTextFile(const wxString& filepath)
{
    wxString content;
    wxTextFile file(filepath);
    
    if (file.Open()) {
        for (size_t i = 0; i < file.GetLineCount(); i++) {
            content += file.GetLine(i);
            if (i < file.GetLineCount() - 1) {
                content += "\n";
            }
        }
        file.Close();
    }
    
    return content;
}

bool FileUtils::WriteTextFile(const wxString& filepath, const wxString& content)
{
    wxTextFile file;
    
    if (FileExists(filepath)) {
        file.Open(filepath);
        file.Clear();
    } else {
        file.Create(filepath);
    }
    
    wxStringTokenizer tokenizer(content, "\n");
    while (tokenizer.HasMoreTokens()) {
        file.AddLine(tokenizer.GetNextToken());
    }
    
    bool success = file.Write();
    file.Close();
    return success;
}

wxArrayString FileUtils::ReadLinesFromFile(const wxString& filepath)
{
    wxArrayString lines;
    wxTextFile file(filepath);
    
    if (file.Open()) {
        for (size_t i = 0; i < file.GetLineCount(); i++) {
            lines.Add(file.GetLine(i));
        }
        file.Close();
    }
    
    return lines;
}

bool FileUtils::WritLinesToFile(const wxString& filepath, const wxArrayString& lines)
{
    wxTextFile file;
    
    if (FileExists(filepath)) {
        file.Open(filepath);
        file.Clear();
    } else {
        file.Create(filepath);
    }
    
    for (const wxString& line : lines) {
        file.AddLine(line);
    }
    
    bool success = file.Write();
    file.Close();
    return success;
}

// Media file utilities
bool FileUtils::IsValidMediaFormat(const wxString& filepath)
{
    if (!FileExists(filepath)) {
        return false;
    }
    
    return IsMediaFile(filepath);
}

wxString FileUtils::GetMimeType(const wxString& filepath)
{
    wxFileType* fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(GetFileExtension(filepath));
    
    if (fileType) {
        wxString mimeType;
        if (fileType->GetMimeType(&mimeType)) {
            delete fileType;
            return mimeType;
        }
        delete fileType;
    }
    
    return wxEmptyString;
}

bool FileUtils::CanPlayFile(const wxString& filepath)
{
    return IsValidMediaFormat(filepath);
}

// File backup utilities
wxString FileUtils::CreateBackupFilename(const wxString& filepath)
{
    wxFileName fn(filepath);
    wxString backup_name = fn.GetName() + "_backup_" + 
                          wxDateTime::Now().Format("%Y%m%d_%H%M%S");
    fn.SetName(backup_name);
    return fn.GetFullPath();
}

bool FileUtils::BackupFile(const wxString& filepath)
{
    if (!FileExists(filepath)) {
        return false;
    }
    
    wxString backup_path = CreateBackupFilename(filepath);
    return CopyFile(filepath, backup_path);
}

bool FileUtils::RestoreBackup(const wxString& filepath)
{
    wxString backup_path = CreateBackupFilename(filepath);
    if (!FileExists(backup_path)) {
        return false;
    }
    
    return CopyFile(backup_path, filepath);
}

// Temporary file utilities
wxString FileUtils::GetTempDirectory()
{
    return wxStandardPaths::Get().GetTempDir();
}

wxString FileUtils::CreateTempFile(const wxString& prefix)
{
    wxString temp_dir = GetTempDirectory();
    wxString temp_name = prefix + "_" + wxDateTime::Now().Format("%Y%m%d_%H%M%S") + 
                        wxString::Format("_%d", wxGetProcessId());
    
    return wxFileName(temp_dir, temp_name).GetFullPath();
}

bool FileUtils::CleanupTempFiles()
{
    wxString temp_dir = GetTempDirectory();
    wxArrayString temp_files = GetFilesInDirectory(temp_dir, false, "wanjplayer_*");
    
    bool all_deleted = true;
    for (const wxString& file : temp_files) {
        if (!DeleteFile(file)) {
            all_deleted = false;
        }
    }
    
    return all_deleted;
}

// File comparison
bool FileUtils::AreFilesEqual(const wxString& file1, const wxString& file2)
{
    if (!FileExists(file1) || !FileExists(file2)) {
        return false;
    }
    
    if (GetFileSize(file1) != GetFileSize(file2)) {
        return false;
    }
    
    wxFileInputStream stream1(file1);
    wxFileInputStream stream2(file2);
    
    if (!stream1.IsOk() || !stream2.IsOk()) {
        return false;
    }
    
    const size_t buffer_size = 4096;
    char buffer1[buffer_size];
    char buffer2[buffer_size];
    
    while (!stream1.Eof() && !stream2.Eof()) {
        stream1.Read(buffer1, buffer_size);
        stream2.Read(buffer2, buffer_size);
        
        if (stream1.LastRead() != stream2.LastRead()) {
            return false;
        }
        
        if (memcmp(buffer1, buffer2, stream1.LastRead()) != 0) {
            return false;
        }
    }
    
    return stream1.Eof() && stream2.Eof();
}

wxString FileUtils::GetFileChecksum(const wxString& filepath)
{
    if (!FileExists(filepath)) {
        return wxEmptyString;
    }
    
    wxFileInputStream stream(filepath);
    if (!stream.IsOk()) {
        return wxEmptyString;
    }
    
    // Simple checksum calculation (not cryptographically secure)
    wxUint32 checksum = 0;
    const size_t buffer_size = 4096;
    char buffer[buffer_size];
    
    while (!stream.Eof()) {
        stream.Read(buffer, buffer_size);
        size_t bytes_read = stream.LastRead();
        
        for (size_t i = 0; i < bytes_read; i++) {
            checksum = ((checksum << 1) | (checksum >> 31)) ^ static_cast<wxUint32>(buffer[i]);
        }
    }
    
    return wxString::Format("%08X", checksum);
}

// Private helper methods
bool FileUtils::HasExtension(const wxString& filepath, const std::vector<wxString>& extensions)
{
    wxString ext = GetFileExtension(filepath);
    return std::find(extensions.begin(), extensions.end(), ext) != extensions.end();
}

wxString FileUtils::NormalizeExtension(const wxString& extension)
{
    wxString normalized = extension.Lower();
    if (normalized.StartsWith(".")) {
        normalized = normalized.Mid(1);
    }
    return normalized;
}

}