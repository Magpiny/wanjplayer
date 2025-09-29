#ifndef __PLAYLIST__HPP
#define __PLAYLIST__HPP

#include "wanjplayer.hpp"

// Forward declarations
namespace utils {
    class QueueManager;
    class FileUtils;
}

namespace gui::player {

class Playlist : public wxListBox
{
public:
    // Constructor
    Playlist(wxWindow* parent, wxWindowID id);
    ~Playlist();

    // Core playlist operations
    void AddItem(const wxString& path);
    void AddMultipleItems(const wxArrayString& paths);
    void RemoveItem(size_t index);
    void RemoveCurrentItem();
    void ClearPlayQueue();
    void MoveItem(size_t from, size_t to);
    
    // Queue management
    wxString GetItem(size_t index) const;
    wxString GetCurrentItem() const;
    unsigned int GetCount() const override;
    bool IsEmpty() const;
    
    // Playback control
    void PlayItemAtIndex(size_t index);
    void PlayNextItem();
    void PlayPreviousItem();
    void PlayCurrentItem();
    
    // Navigation
    bool HasNext() const;
    bool HasPrevious() const;
    size_t GetCurrentIndex() const;
    void SetCurrentIndex(size_t index);
    
    // Playback modes
    enum class RepeatMode { NONE, ONE, ALL };
    enum class ShuffleMode { OFF, ON };
    
    void SetRepeatMode(RepeatMode mode);
    RepeatMode GetRepeatMode() const;
    void SetShuffleMode(ShuffleMode mode);
    ShuffleMode GetShuffleMode() const;
    
    // Media control integration
    wxMediaCtrl* GetMediaCtrl();
    void SetMediaCtrl(wxMediaCtrl* media_ctrl);
    
    // File validation and filtering
    bool IsValidMediaFile(const wxString& path) const;
    void FilterByMediaType(bool video_only = false, bool audio_only = false);
    
    // Playlist management
    void SavePlaylist(const wxString& filepath);
    void LoadPlaylist(const wxString& filepath);
    void ExportPlaylist(const wxString& filepath, const wxString& format = "m3u");
    
    // Search and sorting
    void SearchItems(const wxString& query);
    void SortByName(bool ascending = true);
    void SortByDuration(bool ascending = true);
    void SortByDateAdded(bool ascending = true);
    
    // Statistics
    wxTimeSpan GetTotalDuration() const;
    unsigned int GetVideoCount() const;
    unsigned int GetAudioCount() const;
    
    // Event handling
    void OnMediaFinished(wxMediaEvent& event);
    void OnMediaLoaded(wxMediaEvent& event);
    void OnMediaError(wxMediaEvent& event);

private:
    // Internal data
    std::vector<wxString> play_queue;
    std::vector<wxTimeSpan> item_durations;
    std::vector<wxDateTime> date_added;
    std::vector<bool> is_video_file;
    
    size_t current_index;
    wxMediaCtrl* media_ctrl_ref;
    
    // Queue management utility
    utils::QueueManager* queue_manager;
    
    // Playback state
    bool auto_play_next;
    bool crossfade_enabled;
    int crossfade_duration_ms;
    
    // Event handlers
    void OnKeyDown(wxKeyEvent& event);
    void OnDoubleClick(wxCommandEvent& event);
    void OnListSelection(wxCommandEvent& event);
    void OnRightClick(wxContextMenuEvent& event);
    
    // Helper methods
    void HighlightCurrentTrack();
    void UpdateItemInfo(size_t index);
    void ValidateQueue();
    bool LoadMediaFile(const wxString& path);
    void NotifyPlaybackChange();
    void CreateContextMenu(wxContextMenuEvent& event);
    
    // Internal navigation
    size_t GetNextPlaybackIndex() const;
    size_t GetPreviousPlaybackIndex() const;
    void HandlePlaybackEnd();
    
    // File operations
    void RefreshItemDurations();
    void CacheItemInfo(size_t index, const wxString& path);
    
    // Constants
    static const int MAX_QUEUE_SIZE = 10000;
    static const int DEFAULT_CROSSFADE_DURATION = 3000; // 3 seconds
};

// Playlist file format handlers
class PlaylistFileHandler
{
public:
    // Supported formats
    enum class Format {
        M3U,
        PLS,
        XSPF,
        WPL
    };
    
    static bool CanHandle(const wxString& filepath);
    static Format DetectFormat(const wxString& filepath);
    static wxArrayString LoadPlaylistFile(const wxString& filepath);
    static bool SavePlaylistFile(const wxString& filepath, const wxArrayString& items, Format format);
    
private:
    static wxArrayString LoadM3U(const wxString& filepath);
    static wxArrayString LoadPLS(const wxString& filepath);
    static wxArrayString LoadXSPF(const wxString& filepath);
    static bool SaveM3U(const wxString& filepath, const wxArrayString& items);
    static bool SavePLS(const wxString& filepath, const wxArrayString& items);
    static bool SaveXSPF(const wxString& filepath, const wxArrayString& items);
};

// Playlist item metadata
struct PlaylistItem
{
    wxString filepath;
    wxString title;
    wxString artist;
    wxString album;
    wxTimeSpan duration;
    wxDateTime date_added;
    bool is_video;
    wxULongLong file_size;
    
    PlaylistItem() : is_video(false), file_size(0) {}
    PlaylistItem(const wxString& path) : filepath(path), is_video(false), file_size(0)
    {
        date_added = wxDateTime::Now();
        // File utilities will be called in the implementation
    }
};

// Enhanced playlist with metadata support
class EnhancedPlaylist : public Playlist
{
public:
    EnhancedPlaylist(wxWindow* parent, wxWindowID id);
    
    // Metadata operations
    void SetItemMetadata(size_t index, const PlaylistItem& metadata);
    PlaylistItem GetItemMetadata(size_t index) const;
    void RefreshAllMetadata();
    
    // Advanced features
    void CreateSmartPlaylist(const wxString& criteria);
    void AddFromDirectory(const wxString& directory, bool recursive = false);
    void RemoveDuplicates();
    void RemoveMissingFiles();
    
    // Library integration
    void SyncWithMusicLibrary();
    void UpdateFromLastFM();
    
private:
    std::vector<PlaylistItem> item_metadata;
    
    void ExtractMetadataFromFile(const wxString& filepath, PlaylistItem& item);
    bool MatchesCriteria(const PlaylistItem& item, const wxString& criteria) const;
};

}

#endif // __PLAYLIST_HPP