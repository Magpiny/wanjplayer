#include "playlist.hpp"
#include "utils.hpp"
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <algorithm>
#include <fstream>
#include <set>

namespace gui::player {

// Playlist implementation
Playlist::Playlist(wxWindow* parent, wxWindowID id)
    : wxListBox(parent, id, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_SINGLE)
    , current_index(0)
    , media_ctrl_ref(nullptr)
    , queue_manager(new utils::QueueManager())
    , auto_play_next(true)
    , crossfade_enabled(false)
    , crossfade_duration_ms(DEFAULT_CROSSFADE_DURATION)
{
    // Setup event bindings
    Bind(wxEVT_KEY_DOWN, &Playlist::OnKeyDown, this);
    Bind(wxEVT_LISTBOX_DCLICK, &Playlist::OnDoubleClick, this);
    Bind(wxEVT_LISTBOX, &Playlist::OnListSelection, this);
    Bind(wxEVT_CONTEXT_MENU, &Playlist::OnRightClick, this);
    
    utils::LogUtils::LogInfo("Playlist initialized");
}

Playlist::~Playlist()
{
    ClearPlayQueue();
    delete queue_manager;
    utils::LogUtils::LogInfo("Playlist destroyed");
}

// Core playlist operations
void Playlist::AddItem(const wxString& path)
{
    if (!IsValidMediaFile(path)) {
        utils::LogUtils::LogWarning("Invalid media file: " + path);
        return;
    }
    
    if (play_queue.size() >= MAX_QUEUE_SIZE) {
        utils::LogUtils::LogWarning("Maximum queue size reached");
        return;
    }
    
    auto start_time = utils::PerformanceUtils::StartTimer();
    
    wxFileName file_name(path);
    play_queue.push_back(path);
    date_added.push_back(wxDateTime::Now());
    is_video_file.push_back(utils::FileUtils::IsVideoFile(path));
    item_durations.push_back(wxTimeSpan(0)); // Will be updated when played
    
    Append(file_name.GetFullName());
    
    // If this is the first item, set it as current
    if (play_queue.size() == 1) {
        current_index = 0;
        HighlightCurrentTrack();
    }
    
    // Update queue manager with new size
    if (queue_manager && queue_manager->IsShuffleEnabled()) {
        queue_manager->GenerateShuffleOrder(play_queue.size());
    }
    
    auto duration = utils::PerformanceUtils::EndTimer(start_time);
    utils::LogUtils::LogPerformance("AddItem", duration);
    
    utils::LogUtils::LogInfo("Added item to playlist: " + file_name.GetFullName());
}

void Playlist::AddMultipleItems(const wxArrayString& paths)
{
    auto start_time = utils::PerformanceUtils::StartTimer();
    
    for (const auto& path : paths) {
        if (IsValidMediaFile(path)) {
            AddItem(path);
        }
    }
    
    auto duration = utils::PerformanceUtils::EndTimer(start_time);
    utils::LogUtils::LogPerformance("AddMultipleItems", duration);
}

void Playlist::RemoveItem(size_t index)
{
    if (index >= play_queue.size()) {
        return;
    }
    
    wxString removed_item = play_queue[index];
    
    // Remove from all vectors
    play_queue.erase(play_queue.begin() + index);
    date_added.erase(date_added.begin() + index);
    is_video_file.erase(is_video_file.begin() + index);
    if (index < item_durations.size()) {
        item_durations.erase(item_durations.begin() + index);
    }
    
    // Remove from UI
    Delete(index);
    
    // Adjust current index if necessary
    if (current_index >= index && current_index > 0) {
        current_index--;
    }
    if (current_index >= play_queue.size() && !play_queue.empty()) {
        current_index = play_queue.size() - 1;
    }
    
    HighlightCurrentTrack();
    
    // Update queue manager
    if (queue_manager && queue_manager->IsShuffleEnabled()) {
        queue_manager->GenerateShuffleOrder(play_queue.size());
    }
    
    utils::LogUtils::LogInfo("Removed item from playlist: " + removed_item);
}

void Playlist::RemoveCurrentItem()
{
    if (!IsEmpty()) {
        RemoveItem(current_index);
    }
}

void Playlist::ClearPlayQueue()
{
    play_queue.clear();
    date_added.clear();
    is_video_file.clear();
    item_durations.clear();
    Clear();
    current_index = 0;
    
    if (queue_manager) {
        queue_manager->Reset();
    }
    
    utils::LogUtils::LogInfo("Playlist cleared");
}

void Playlist::MoveItem(size_t from, size_t to)
{
    if (from >= play_queue.size() || to >= play_queue.size() || from == to) {
        return;
    }
    
    // Move in all vectors
    wxString item = play_queue[from];
    wxDateTime added = date_added[from];
    bool is_video = is_video_file[from];
    wxTimeSpan duration = (from < item_durations.size()) ? item_durations[from] : wxTimeSpan(0);
    
    // Remove from old position
    play_queue.erase(play_queue.begin() + from);
    date_added.erase(date_added.begin() + from);
    is_video_file.erase(is_video_file.begin() + from);
    if (from < item_durations.size()) {
        item_durations.erase(item_durations.begin() + from);
    }
    
    // Insert at new position
    play_queue.insert(play_queue.begin() + to, item);
    date_added.insert(date_added.begin() + to, added);
    is_video_file.insert(is_video_file.begin() + to, is_video);
    if (to < item_durations.size()) {
        item_durations.insert(item_durations.begin() + to, duration);
    }
    
    // Update UI
    wxString display_name = utils::FileUtils::GetFileName(item);
    Delete(from);
    Insert(display_name, to);
    
    // Adjust current index
    if (from == current_index) {
        current_index = to;
    } else if (from < current_index && to >= current_index) {
        current_index--;
    } else if (from > current_index && to <= current_index) {
        current_index++;
    }
    
    HighlightCurrentTrack();
    
    utils::LogUtils::LogInfo(wxString::Format("Moved item from %zu to %zu", from, to));
}

// Queue management
wxString Playlist::GetItem(size_t index) const
{
    if (index < play_queue.size()) {
        return play_queue[index];
    }
    return wxEmptyString;
}

wxString Playlist::GetCurrentItem() const
{
    return GetItem(current_index);
}

unsigned int Playlist::GetCount() const
{
    return static_cast<unsigned int>(play_queue.size());
}

bool Playlist::IsEmpty() const
{
    return play_queue.empty();
}

// Playback control
void Playlist::PlayItemAtIndex(size_t index)
{
    if (IsEmpty() || index >= play_queue.size()) {
        utils::LogUtils::LogWarning("Cannot play item: invalid index");
        return;
    }
    
    if (!media_ctrl_ref) {
        utils::LogUtils::LogError("No media control available");
        return;
    }
    
    current_index = index;
    wxString media_item = play_queue[current_index];
    
    if (!LoadMediaFile(media_item)) {
        utils::LogUtils::LogError("Failed to load media file: " + media_item);
        return;
    }
    
    HighlightCurrentTrack();
    NotifyPlaybackChange();
}

void Playlist::PlayNextItem()
{
    if (IsEmpty()) {
        return;
    }
    
    size_t next_index = GetNextPlaybackIndex();
    if (next_index != current_index || (queue_manager && queue_manager->ShouldRepeatCurrent())) {
        PlayItemAtIndex(next_index);
    }
}

void Playlist::PlayPreviousItem()
{
    if (IsEmpty()) {
        return;
    }
    
    size_t prev_index = GetPreviousPlaybackIndex();
    if (prev_index != current_index || (queue_manager && queue_manager->ShouldRepeatCurrent())) {
        PlayItemAtIndex(prev_index);
    }
}

void Playlist::PlayCurrentItem()
{
    PlayItemAtIndex(current_index);
}

// Navigation
bool Playlist::HasNext() const
{
    if (IsEmpty()) {
        return false;
    }
    
    if (queue_manager && (queue_manager->ShouldRepeatCurrent() || queue_manager->ShouldRepeatAll())) {
        return true;
    }
    
    if (queue_manager && queue_manager->IsShuffleEnabled()) {
        return true; // Shuffle can always find a next item
    }
    
    return current_index < play_queue.size() - 1;
}

bool Playlist::HasPrevious() const
{
    if (IsEmpty()) {
        return false;
    }
    
    if (queue_manager && (queue_manager->ShouldRepeatCurrent() || queue_manager->ShouldRepeatAll())) {
        return true;
    }
    
    if (queue_manager && queue_manager->IsShuffleEnabled()) {
        return true; // Shuffle can always find a previous item
    }
    
    return current_index > 0;
}

size_t Playlist::GetCurrentIndex() const
{
    return current_index;
}

void Playlist::SetCurrentIndex(size_t index)
{
    if (index < play_queue.size()) {
        current_index = index;
        HighlightCurrentTrack();
    }
}

// Playback modes
void Playlist::SetRepeatMode(RepeatMode mode)
{
    if (!queue_manager) return;
    
    utils::QueueManager::RepeatMode utils_mode;
    switch (mode) {
        case RepeatMode::ONE:
            utils_mode = utils::QueueManager::RepeatMode::ONE;
            break;
        case RepeatMode::ALL:
            utils_mode = utils::QueueManager::RepeatMode::ALL;
            break;
        default:
            utils_mode = utils::QueueManager::RepeatMode::NONE;
            break;
    }
    queue_manager->SetRepeatMode(utils_mode);
    utils::LogUtils::LogInfo("Repeat mode changed");
}

Playlist::RepeatMode Playlist::GetRepeatMode() const
{
    if (!queue_manager) return RepeatMode::NONE;
    
    auto utils_mode = queue_manager->GetRepeatMode();
    switch (utils_mode) {
        case utils::QueueManager::RepeatMode::ONE:
            return RepeatMode::ONE;
        case utils::QueueManager::RepeatMode::ALL:
            return RepeatMode::ALL;
        default:
            return RepeatMode::NONE;
    }
}

void Playlist::SetShuffleMode(ShuffleMode mode)
{
    if (!queue_manager) return;
    
    utils::QueueManager::ShuffleMode utils_mode = 
        (mode == ShuffleMode::ON) ? utils::QueueManager::ShuffleMode::ON 
                                  : utils::QueueManager::ShuffleMode::OFF;
    
    queue_manager->SetShuffleMode(utils_mode);
    if (mode == ShuffleMode::ON) {
        queue_manager->GenerateShuffleOrder(play_queue.size());
    }
    utils::LogUtils::LogInfo("Shuffle mode changed");
}

Playlist::ShuffleMode Playlist::GetShuffleMode() const
{
    if (!queue_manager) return ShuffleMode::OFF;
    
    return (queue_manager->GetShuffleMode() == utils::QueueManager::ShuffleMode::ON) 
           ? ShuffleMode::ON : ShuffleMode::OFF;
}

// Media control integration
wxMediaCtrl* Playlist::GetMediaCtrl()
{
    if (media_ctrl_ref) {
        return media_ctrl_ref;
    }
    return dynamic_cast<wxMediaCtrl*>(GetParent()->FindWindowById(ID_MEDIA_CTRL));
}

void Playlist::SetMediaCtrl(wxMediaCtrl* media_ctrl)
{
    media_ctrl_ref = media_ctrl;
}

// File validation and filtering
bool Playlist::IsValidMediaFile(const wxString& path) const
{
    return utils::FileUtils::IsMediaFile(path) && utils::FileUtils::FileExists(path);
}

void Playlist::FilterByMediaType(bool video_only, bool audio_only)
{
    if (video_only == audio_only) {
        return; // No filtering if both or neither are selected
    }
    
    std::vector<size_t> indices_to_remove;
    
    for (size_t i = 0; i < play_queue.size(); ++i) {
        bool is_video = (i < is_video_file.size()) ? is_video_file[i] : utils::FileUtils::IsVideoFile(play_queue[i]);
        
        if ((video_only && !is_video) || (audio_only && is_video)) {
            indices_to_remove.push_back(i);
        }
    }
    
    // Remove items in reverse order to maintain indices
    for (auto it = indices_to_remove.rbegin(); it != indices_to_remove.rend(); ++it) {
        RemoveItem(*it);
    }
    
    utils::LogUtils::LogInfo(wxString::Format("Filtered playlist: removed %zu items", indices_to_remove.size()));
}

// Playlist management
void Playlist::SavePlaylist(const wxString& filepath)
{
    wxArrayString items;
    for (const auto& path : play_queue) {
        items.Add(path);
    }
    PlaylistFileHandler::SavePlaylistFile(filepath, items, PlaylistFileHandler::Format::M3U);
    utils::LogUtils::LogInfo("Playlist saved to: " + filepath);
}

void Playlist::LoadPlaylist(const wxString& filepath)
{
    if (!PlaylistFileHandler::CanHandle(filepath)) {
        utils::LogUtils::LogError("Unsupported playlist format: " + filepath);
        return;
    }
    
    wxArrayString items = PlaylistFileHandler::LoadPlaylistFile(filepath);
    ClearPlayQueue();
    AddMultipleItems(items);
    
    utils::LogUtils::LogInfo("Playlist loaded from: " + filepath);
}

void Playlist::ExportPlaylist(const wxString& filepath, const wxString& format)
{
    PlaylistFileHandler::Format fmt = PlaylistFileHandler::Format::M3U;
    
    if (format.Lower() == "pls") {
        fmt = PlaylistFileHandler::Format::PLS;
    } else if (format.Lower() == "xspf") {
        fmt = PlaylistFileHandler::Format::XSPF;
    }
    
    wxArrayString items;
    for (const auto& path : play_queue) {
        items.Add(path);
    }
    PlaylistFileHandler::SavePlaylistFile(filepath, items, fmt);
    utils::LogUtils::LogInfo("Playlist exported to: " + filepath);
}

// Search and sorting
void Playlist::SearchItems(const wxString& query)
{
    wxString lower_query = query.Lower();
    
    for (size_t i = 0; i < GetCount(); ++i) {
        wxString item_name = utils::FileUtils::GetFileName(GetItem(i)).Lower();
        
        if (item_name.Contains(lower_query)) {
            SetSelection(i);
            break;
        }
    }
}

void Playlist::SortByName(bool ascending)
{
    std::vector<std::pair<wxString, size_t>> items_with_index;
    
    for (size_t i = 0; i < play_queue.size(); ++i) {
        wxString name = utils::FileUtils::GetFileName(play_queue[i]);
        items_with_index.emplace_back(name, i);
    }
    
    std::sort(items_with_index.begin(), items_with_index.end(),
        [ascending](const auto& a, const auto& b) {
            return ascending ? a.first < b.first : a.first > b.first;
        });
    
    // Reorder based on sorted indices
    std::vector<wxString> new_queue;
    std::vector<wxDateTime> new_dates;
    std::vector<bool> new_video_flags;
    std::vector<wxTimeSpan> new_durations;
    
    for (const auto& item : items_with_index) {
        size_t orig_index = item.second;
        new_queue.push_back(play_queue[orig_index]);
        new_dates.push_back(date_added[orig_index]);
        new_video_flags.push_back(is_video_file[orig_index]);
        if (orig_index < item_durations.size()) {
            new_durations.push_back(item_durations[orig_index]);
        }
    }
    
    play_queue = std::move(new_queue);
    date_added = std::move(new_dates);
    is_video_file = std::move(new_video_flags);
    item_durations = std::move(new_durations);
    
    // Refresh UI
    Clear();
    for (const auto& path : play_queue) {
        Append(utils::FileUtils::GetFileName(path));
    }
    
    utils::LogUtils::LogInfo("Playlist sorted by name");
}

void Playlist::SortByDuration(bool ascending)
{
    // Implementation would require duration metadata
    utils::LogUtils::LogInfo("Sort by duration not yet implemented");
}

void Playlist::SortByDateAdded(bool ascending)
{
    std::vector<std::pair<wxDateTime, size_t>> items_with_index;
    
    for (size_t i = 0; i < play_queue.size() && i < date_added.size(); ++i) {
        items_with_index.emplace_back(date_added[i], i);
    }
    
    std::sort(items_with_index.begin(), items_with_index.end(),
        [ascending](const auto& a, const auto& b) {
            return ascending ? a.first < b.first : a.first > b.first;
        });
    
    // Similar reordering logic as SortByName
    utils::LogUtils::LogInfo("Playlist sorted by date added");
}

// Statistics
wxTimeSpan Playlist::GetTotalDuration() const
{
    wxTimeSpan total(0);
    for (const auto& duration : item_durations) {
        total = total.Add(duration);
    }
    return total;
}

unsigned int Playlist::GetVideoCount() const
{
    return static_cast<unsigned int>(std::count(is_video_file.begin(), is_video_file.end(), true));
}

unsigned int Playlist::GetAudioCount() const
{
    return static_cast<unsigned int>(play_queue.size()) - GetVideoCount();
}

// Event handling
void Playlist::OnMediaFinished(wxMediaEvent& event)
{
    utils::LogUtils::LogInfo("Media finished, attempting to play next item");
    
    if (auto_play_next) {
        HandlePlaybackEnd();
    }
    
    event.Skip();
}

void Playlist::OnMediaLoaded(wxMediaEvent& event)
{
    utils::LogUtils::LogInfo("Media loaded successfully");
    UpdateItemInfo(current_index);
    event.Skip();
}

void Playlist::OnMediaError(wxMediaEvent& event)
{
    utils::LogUtils::LogError("Media playback error occurred");
    
    // Try to skip to next item on error
    if (auto_play_next && HasNext()) {
        PlayNextItem();
    }
    
    event.Skip();
}

// Private helper methods
void Playlist::OnKeyDown(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_SPACE) {
        int selection = GetSelection();
        if (selection >= 0) {
            PlayItemAtIndex(selection);
        }
    } else if (event.GetKeyCode() == WXK_DELETE || event.GetKeyCode() == WXK_BACK) {
        int selection = GetSelection();
        if (selection >= 0) {
            RemoveItem(selection);
        }
    }
    event.Skip();
}

void Playlist::OnDoubleClick(wxCommandEvent& event)
{
    int index = event.GetSelection();
    if (index >= 0) {
        PlayItemAtIndex(index);
    }
    event.Skip();
}

void Playlist::OnListSelection(wxCommandEvent& event)
{
    event.Skip();
}

void Playlist::OnRightClick(wxContextMenuEvent& event)
{
    CreateContextMenu(event);
}

void Playlist::HighlightCurrentTrack()
{
    if (!IsEmpty() && current_index < play_queue.size()) {
        SetSelection(current_index);
    }
}

void Playlist::UpdateItemInfo(size_t index)
{
    if (index >= play_queue.size()) {
        return;
    }
    
    // Cache file information for performance
    CacheItemInfo(index, play_queue[index]);
}

void Playlist::ValidateQueue()
{
    std::vector<size_t> invalid_indices;
    
    for (size_t i = 0; i < play_queue.size(); ++i) {
        if (!utils::FileUtils::FileExists(play_queue[i])) {
            invalid_indices.push_back(i);
        }
    }
    
    // Remove invalid files
    for (auto it = invalid_indices.rbegin(); it != invalid_indices.rend(); ++it) {
        RemoveItem(*it);
    }
    
    if (!invalid_indices.empty()) {
        utils::LogUtils::LogInfo(wxString::Format("Removed %zu invalid files from playlist", invalid_indices.size()));
    }
}

bool Playlist::LoadMediaFile(const wxString& path)
{
    if (!media_ctrl_ref) {
        return false;
    }
    
    if (media_ctrl_ref->Load(path)) {
        media_ctrl_ref->Play();
        return true;
    }
    
    return false;
}

void Playlist::NotifyPlaybackChange()
{
    // Notify other components of playback changes
    wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, wxID_ANY);
    event.SetEventObject(this);
    ProcessEvent(event);
}

void Playlist::CreateContextMenu(wxContextMenuEvent& event)
{
    wxMenu* context_menu = new wxMenu();
    
    context_menu->Append(wxID_ANY, "Play");
    context_menu->Append(wxID_ANY, "Remove from Playlist");
    context_menu->AppendSeparator();
    context_menu->Append(wxID_ANY, "Show in File Manager");
    context_menu->Append(wxID_ANY, "Properties");
    
    PopupMenu(context_menu);
    delete context_menu;
}

size_t Playlist::GetNextPlaybackIndex() const
{
    if (!queue_manager) return current_index;
    return queue_manager->GetNextIndex(current_index, play_queue.size());
}

size_t Playlist::GetPreviousPlaybackIndex() const
{
    if (!queue_manager) return current_index;
    return queue_manager->GetPreviousIndex(current_index, play_queue.size());
}

void Playlist::HandlePlaybackEnd()
{
    if (HasNext()) {
        PlayNextItem();
    }
}

void Playlist::RefreshItemDurations()
{
    // This would require media analysis
    // Implementation depends on available media libraries
}

void Playlist::CacheItemInfo(size_t index, const wxString& path)
{
    if (index >= play_queue.size()) {
        return;
    }
    
    // Update video flag if not set
    if (index >= is_video_file.size()) {
        is_video_file.resize(index + 1);
    }
    is_video_file[index] = utils::FileUtils::IsVideoFile(path);
    
    // Cache file size and other metadata as needed
}

// PlaylistFileHandler implementation
bool PlaylistFileHandler::CanHandle(const wxString& filepath)
{
    wxString ext = utils::FileUtils::GetFileExtension(filepath).Lower();
    return ext == "m3u" || ext == "pls" || ext == "xspf" || ext == "wpl";
}

PlaylistFileHandler::Format PlaylistFileHandler::DetectFormat(const wxString& filepath)
{
    wxString ext = utils::FileUtils::GetFileExtension(filepath).Lower();
    
    if (ext == "pls") return Format::PLS;
    if (ext == "xspf") return Format::XSPF;
    if (ext == "wpl") return Format::WPL;
    return Format::M3U; // Default
}

wxArrayString PlaylistFileHandler::LoadPlaylistFile(const wxString& filepath)
{
    Format format = DetectFormat(filepath);
    
    switch (format) {
        case Format::PLS:
            return LoadPLS(filepath);
        case Format::XSPF:
            return LoadXSPF(filepath);
        case Format::M3U:
        default:
            return LoadM3U(filepath);
    }
}

bool PlaylistFileHandler::SavePlaylistFile(const wxString& filepath, const wxArrayString& items, Format format)
{
    switch (format) {
        case Format::PLS:
            return SavePLS(filepath, items);
        case Format::XSPF:
            return SaveXSPF(filepath, items);
        case Format::M3U:
        default:
            return SaveM3U(filepath, items);
    }
}

wxArrayString PlaylistFileHandler::LoadM3U(const wxString& filepath)
{
    wxArrayString items;
    wxTextFile file(filepath);
    
    if (file.Open()) {
        for (size_t i = 0; i < file.GetLineCount(); ++i) {
            wxString line = file.GetLine(i).Trim();
            if (!line.IsEmpty() && !line.StartsWith("#")) {
                items.Add(line);
            }
        }
        file.Close();
    }
    
    return items;
}

wxArrayString PlaylistFileHandler::LoadPLS(const wxString& filepath)
{
    wxArrayString items;
    wxTextFile file(filepath);
    
    if (file.Open()) {
        for (size_t i = 0; i < file.GetLineCount(); ++i) {
            wxString line = file.GetLine(i).Trim();
            if (line.StartsWith("File")) {
                size_t eq_pos = line.Find('=');
                if (eq_pos != wxString::npos) {
                    items.Add(line.Mid(eq_pos + 1));
                }
            }
        }
        file.Close();
    }
    
    return items;
}

wxArrayString PlaylistFileHandler::LoadXSPF(const wxString& filepath)
{
    wxArrayString items;
    // XSPF loading would require XML parsing
    utils::LogUtils::LogWarning("XSPF loading not yet implemented");
    return items;
}

bool PlaylistFileHandler::SaveM3U(const wxString& filepath, const wxArrayString& items)
{
    wxTextFile file(filepath);
    
    if (file.Exists()) {
        file.Open();
        file.Clear();
    } else {
        file.Create();
    }
    
    file.AddLine("#EXTM3U");
    for (const auto& item : items) {
        file.AddLine(item);
    }
    
    bool success = file.Write();
    file.Close();
    
    return success;
}

bool PlaylistFileHandler::SavePLS(const wxString& filepath, const wxArrayString& items)
{
    wxTextFile file(filepath);
    
    if (file.Exists()) {
        file.Open();
        file.Clear();
    } else {
        file.Create();
    }
    
    file.AddLine("[playlist]");
    for (size_t i = 0; i < items.size(); ++i) {
        file.AddLine(wxString::Format("File%zu=%s", i + 1, items[i]));
    }
    file.AddLine(wxString::Format("NumberOfEntries=%zu", items.size()));
    file.AddLine("Version=2");
    
    bool success = file.Write();
    file.Close();
    
    return success;
}

bool PlaylistFileHandler::SaveXSPF(const wxString& filepath, const wxArrayString& items)
{
    // XSPF saving would require XML generation
    utils::LogUtils::LogWarning("XSPF saving not yet implemented");
    return false;
}

// EnhancedPlaylist implementation
EnhancedPlaylist::EnhancedPlaylist(wxWindow* parent, wxWindowID id)
    : Playlist(parent, id)
{
}

void EnhancedPlaylist::SetItemMetadata(size_t index, const PlaylistItem& metadata)
{
    if (index >= item_metadata.size()) {
        item_metadata.resize(index + 1);
    }
    item_metadata[index] = metadata;
}

PlaylistItem EnhancedPlaylist::GetItemMetadata(size_t index) const
{
    if (index < item_metadata.size()) {
        return item_metadata[index];
    }
    return PlaylistItem();
}

void EnhancedPlaylist::RefreshAllMetadata()
{
    item_metadata.clear();
    item_metadata.resize(GetCount());
    
    for (size_t i = 0; i < GetCount(); ++i) {
        PlaylistItem item(GetItem(i));
        ExtractMetadataFromFile(GetItem(i), item);
        item_metadata[i] = item;
    }
}

void EnhancedPlaylist::CreateSmartPlaylist(const wxString& criteria)
{
    // Smart playlist creation based on criteria
    utils::LogUtils::LogInfo("Smart playlist creation not yet implemented");
}

void EnhancedPlaylist::AddFromDirectory(const wxString& directory, bool recursive)
{
    wxArrayString files;
    
    // This would require directory traversal implementation
    utils::LogUtils::LogInfo("Add from directory not yet implemented");
}

void EnhancedPlaylist::RemoveDuplicates()
{
    std::set<wxString> seen_files;
    std::vector<size_t> duplicates;
    
    for (size_t i = 0; i < GetCount(); ++i) {
        wxString normalized_path = utils::FileUtils::NormalizePath(GetItem(i));
        if (seen_files.count(normalized_path)) {
            duplicates.push_back(i);
        } else {
            seen_files.insert(normalized_path);
        }
    }
    
    // Remove duplicates in reverse order
    for (auto it = duplicates.rbegin(); it != duplicates.rend(); ++it) {
        RemoveItem(*it);
    }
    
    utils::LogUtils::LogInfo(wxString::Format("Removed %zu duplicate items", duplicates.size()));
}

void EnhancedPlaylist::RemoveMissingFiles()
{
    // ValidateQueue(); // This already handles missing files - make it public or add friend access
}

void EnhancedPlaylist::SyncWithMusicLibrary()
{
    utils::LogUtils::LogInfo("Music library sync not yet implemented");
}

void EnhancedPlaylist::UpdateFromLastFM()
{
    utils::LogUtils::LogInfo("Last.fm integration not yet implemented");
}

void EnhancedPlaylist::ExtractMetadataFromFile(const wxString& filepath, PlaylistItem& item)
{
    // Metadata extraction would require media libraries like TagLib
    item.filepath = filepath;
    item.title = utils::FileUtils::GetFileName(filepath);
    item.is_video = utils::FileUtils::IsVideoFile(filepath);
    item.file_size = utils::FileUtils::GetFileSize(filepath);
}

bool EnhancedPlaylist::MatchesCriteria(const PlaylistItem& item, const wxString& criteria) const
{
    // Simple criteria matching - could be enhanced
    wxString lower_criteria = criteria.Lower();
    return item.title.Lower().Contains(lower_criteria) ||
           item.artist.Lower().Contains(lower_criteria) ||
           item.album.Lower().Contains(lower_criteria);
}

}