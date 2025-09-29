# WanjPlayer v0.1.0 Technical Documentation

**Version:** 0.1.0  
**Author:** Wanjare Samuel  
**Date:** September 2024  
**License:** MIT  

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Class Hierarchy](#class-hierarchy)
- [Core Components](#core-components)
- [Data Flow](#data-flow)
- [Event System](#event-system)
- [Media Pipeline](#media-pipeline)
- [Build System](#build-system)
- [Environment Management](#environment-management)
- [API Reference](#api-reference)
- [Extension Guide](#extension-guide)
- [Debugging Guide](#debugging-guide)
- [Performance Considerations](#performance-considerations)

## Overview

WanjPlayer is a cross-platform media player built with modern C++23, wxWidgets GUI framework, and GStreamer media backend. It provides a clean, user-friendly interface for playing audio and video files with advanced playlist management and environment compatibility features.

### Key Features

- **Cross-platform GUI**: Built with wxWidgets 3.2.6+ for native look and feel
- **Media Playback**: Supports audio/video via wxMediaCtrl with GStreamer backend
- **Smart Environment**: Automatic Wayland/X11 detection and compatibility
- **Advanced Playlist**: Queue management with navigation controls
- **Precision Controls**: High-resolution seek slider (0.1-second precision)
- **Modern C++**: Uses C++23 features and modern development practices

### Technology Stack

```
Application Layer:    WanjPlayer (wxApp)
GUI Framework:        wxWidgets 3.2.6
Media Backend:        GStreamer (via wxMediaCtrl)
Language:             C++23
Build System:         CMake 3.30.3+
Platform:             Linux (Wayland/X11)
```

## System Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────┐
│                 WanjPlayer App                      │
├─────────────────────────────────────────────────────┤
│  ┌─────────────┐ ┌──────────────┐ ┌─────────────┐  │
│  │   Menubar   │ │  PlayerFrame │ │  StatusBar  │  │
│  │             │ │              │ │             │  │
│  │ - File      │ │ - Layout     │ │ - Duration  │  │
│  │ - Playback  │ │ - Events     │ │ - Status    │  │
│  │ - Help      │ │ - Controls   │ │ - Messages  │  │
│  └─────────────┘ └──────────────┘ └─────────────┘  │
├─────────────────────────────────────────────────────┤
│  ┌─────────────┐                  ┌─────────────┐  │
│  │  Playlist   │                  │MediaControls│  │
│  │             │                  │             │  │
│  │ - Queue     │ ←─── Events ───→ │ - Play/Stop │  │
│  │ - Navigate  │                  │ - Seek      │  │
│  │ - Selection │                  │ - Volume    │  │
│  └─────────────┘                  └─────────────┘  │
├─────────────────────────────────────────────────────┤
│              wxMediaCtrl (GStreamer)                │
├─────────────────────────────────────────────────────┤
│          Environment Compatibility Layer           │
│     (Wayland/X11 Detection & Configuration)        │
└─────────────────────────────────────────────────────┘
```

### Component Interaction Diagram

```
       ┌─────────────┐
       │ WanjPlayer  │ (wxApp)
       │   (main)    │
       └──────┬──────┘
              │ creates
              ▼
       ┌─────────────┐
       │ PlayerFrame │ (wxFrame)
       │  (window)   │
       └──────┬──────┘
              │ contains
              ▼
   ┌─────────────────────────────────┐
   │        UI Components            │
   │                                 │
   │  ┌─────────────┐ ┌─────────────┐│
   │  │   Menubar   │ │  StatusBar  ││
   │  └─────────────┘ └─────────────┘│
   │                                 │
   │  ┌─────────────┐ ┌─────────────┐│
   │  │  Playlist   │ │MediaControls││
   │  │             │ │             ││
   │  │ ┌─────────┐ │ │ ┌─────────┐ ││
   │  │ │play_queue│ │ │ │ Timer   │ ││
   │  │ └─────────┘ │ │ └─────────┘ ││
   │  └─────────────┘ └─────────────┘│
   │           │              │      │
   │           └──────┬───────┘      │
   │                  │              │
   │         ┌─────────────┐         │
   │         │wxMediaCtrl │         │
   │         │(GStreamer) │         │
   │         └─────────────┘         │
   └─────────────────────────────────┘
```

## Class Hierarchy

### Core Classes Structure

```
wxApp
└── WanjPlayer
    └── creates → PlayerFrame (wxFrame)
                  ├── gui::Menubar
                  ├── gui::StatusBar
                  ├── gui::player::MediaControls (wxPanel)
                  │   ├── wxButton (play, pause, stop, prev, next)
                  │   ├── wxSlider (volume, position)
                  │   ├── wxStaticText (time labels)
                  │   └── wxTimer (update timer)
                  ├── gui::player::Playlist (wxListBox)
                  │   └── std::vector<wxString> (play_queue)
                  ├── wxMediaCtrl (media playback)
                  └── wxSplitterWindow (layout manager)
```

### Namespace Organization

```cpp
// Global namespace
class WanjPlayer : public wxApp
class PlayerFrame : public wxFrame

// GUI namespace
namespace gui {
    class Menubar
    class StatusBar
    
    // Player-specific components
    namespace player {
        class MediaControls : public wxPanel
        class Playlist : public wxListBox
    }
}
```

## Core Components

### 1. WanjPlayer (Application Class)

**File:** `src/wanjplayer.cpp`, `include/wanjplayer.hpp`

```cpp
class WanjPlayer : public wxApp
{
public:
    bool OnInit() override;
};
```

**Purpose:** Application entry point and initialization

**Key Responsibilities:**
- Environment variable setup for video compatibility
- Image handler initialization
- Main window creation and display

**Critical Code:**
```cpp
bool WanjPlayer::OnInit() {
    // Environment setup for Wayland/X11 compatibility
    wxSetEnv("GDK_BACKEND", "x11");
    wxSetEnv("GST_GL_DISABLED", "1");
    wxSetEnv("LIBGL_ALWAYS_SOFTWARE", "1");
    
    // Initialize GUI
    wxInitAllImageHandlers();
    PlayerFrame* frame = new PlayerFrame();
    frame->Show(true);
    return true;
}
```

### 2. PlayerFrame (Main Window)

**File:** `src/wanjplayer.cpp`, `include/wanjplayer.hpp`

```cpp
class PlayerFrame : public wxFrame
{
private:
    // UI Components
    wxPanel* video_canvas_pane;
    wxMediaCtrl* media_ctrl;
    gui::player::MediaControls* player_ctrls;
    gui::player::Playlist* playlist;
    gui::StatusBar* status_bar;
    wxSplitterWindow* splitter;
    
    // State
    bool playlist_visible;
    std::size_t current_index;
    
public:
    PlayerFrame();
    
private:
    // Event handlers for UI, media, and playlist operations
    void OnFileOpen(wxCommandEvent& event);
    void OnMediaLoaded(wxMediaEvent& event);
    void OnTogglePlaylist(wxCommandEvent& event);
};
```

**Layout Structure:**
```
PlayerFrame
├── Menubar (top)
├── StatusBar (bottom)
└── wxSplitterWindow (main area)
    ├── Playlist Panel (left, collapsible)
    │   ├── Toggle Button
    │   └── Playlist (wxListBox)
    └── Video Canvas Panel (right)
        ├── wxMediaCtrl (media display)
        └── MediaControls (bottom)
```

### 3. MediaControls (Playback Interface)

**File:** `src/media_ctrls.cpp`, `include/media_ctrls.hpp`

```cpp
class MediaControls : public wxPanel
{
private:
    wxMediaCtrl* _pmedia_ctrl;          // Media control reference
    
    // Control buttons
    wxButton* btn_play, *btn_pause, *btn_stop;
    wxButton* btn_next, *btn_prev;
    
    // Sliders
    wxSlider* slider_volume;            // Volume: 0-100
    wxSlider* slider_playback_position; // Position: 0-100000
    
    // Time display
    wxStaticText* label_current_time;   // "MM:SS"
    wxStaticText* label_total_time;     // "MM:SS"
    
    // Update mechanism
    wxTimer* update_timer;              // 100ms intervals
    
    // State tracking
    wxFileOffset media_duration;       // Total duration (ms)
    wxFileOffset media_position;       // Current position (ms)
    
public:
    MediaControls(wxPanel* parent, wxMediaCtrl* media_ctrl);
    void UpdateDuration();
    void SetPlaylist(Playlist* playlist);
    void SetStatusBar(gui::StatusBar* status_bar);
};
```

**Key Features:**
- **High-precision seeking**: 100,000 slider steps for 0.1-second precision
- **Real-time updates**: Timer-driven position and duration updates
- **Cross-component communication**: Integrates with playlist and status bar

### 4. Playlist (Queue Management)

**File:** `src/playlist.cpp`, `include/playlist.hpp`

```cpp
class Playlist : public wxListBox
{
private:
    std::vector<wxString> play_queue;   // File paths storage
    size_t current_index;               // Currently playing item
    wxMediaCtrl* media_ctrl_ref;        // Reference to media control
    
public:
    // Queue management
    void add_item(const wxString& path);
    void clear_play_queue();
    
    // Navigation
    void play_next_item_in_queue();
    void play_previous_item_in_queue();
    void play_item_at_index(size_t index);
    
    // State queries
    bool has_next() const;
    bool has_previous() const;
    size_t get_current_index() const;
    
    // Events
    void OnDoubleClick(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
};
```

**Data Flow:**
```
File Selection → add_item() → play_queue.push_back()
                           ↓
User Interaction → OnDoubleClick() → play_item_at_index()
                                  ↓
Media Loading → media_ctrl_ref->Load() → highlight_current_track()
```

### 5. StatusBar (Information Display)

**File:** `src/statusbar.cpp`, `include/statusbar.hpp`

```cpp
class StatusBar
{
private:
    wxFrame* _parent;
    
public:
    StatusBar(wxFrame* parent);
    void create_statusbar();
    void update_file_info(const wxString& filename, wxFileOffset duration_ms);
    void update_playback_info(const wxString& status);
    void set_duration_display(const wxString& duration_str);
};
```

**Status Fields:**
```
[Filename.ext] [Playing/Paused/Stopped] [Current Time / Total Time] [System Messages]
      0                    1                        2                      3
```

## Data Flow

### Media Loading Flow

```
1. User Action (File → Open Files)
   │
   ▼
2. PlayerFrame::OnFilesOpen()
   │
   ▼
3. wxFileDialog → GetPaths()
   │
   ▼
4. For each selected file:
   playlist->add_item(path)
   │
   ▼
5. Playlist::add_item()
   ├── play_queue.push_back(path)
   ├── Append(filename)  // Add to UI list
   └── If first item: play_item_at_index(0)
   │
   ▼
6. Playlist::play_item_at_index()
   ├── current_index = index
   ├── media_ctrl_ref->Load(path)
   └── highlight_current_track()
   │
   ▼
7. wxMediaCtrl loads file
   │
   ▼
8. EVT_MEDIA_LOADED event fired
   │
   ▼
9. PlayerFrame::OnMediaLoaded()
   ├── player_ctrls->UpdateDuration()
   └── status_bar->update_file_info()
```

### Playback Control Flow

```
User clicks Play Button
│
▼
MediaControls::OnPlay()
│
▼
_pmedia_ctrl->Play()
│
▼
wxMediaCtrl starts playback
│
▼
Timer starts (100ms intervals)
│
▼
MediaControls::OnUpdateTimer()
├── Update slider position
├── Update time labels
└── Update status bar
```

### Seeking Flow

```
User drags seek slider
│
▼
EVT_SLIDER event
│
▼
MediaControls::OnPositionSliderChange()
├── slider_value = slider_playback_position->GetValue()
├── new_position = slider_value * 100  // Convert to milliseconds
├── _pmedia_ctrl->Seek(new_position)
└── UpdateTimeDisplay()
```

## Event System

### Event ID Enumeration

```cpp
enum {
    ID_OPENFILE = wxID_HIGHEST + 1,
    ID_OPEN_FILES,
    ID_PREFS,
    ID_LICENSE,
    ID_MEDIA_LOADED,
    ID_MEDIA_STOP,
    ID_MEDIA_PLAY,
    ID_MEDIA_PAUSE,
    ID_MEDIA_FINISHED,
    ID_MEDIA_CANVAS,
    ID_MEDIA_CTRL,
    ID_TOGGLE_PLAYLIST
};
```

### Event Binding Pattern

```cpp
// In PlayerFrame constructor
Bind(wxEVT_MENU, &PlayerFrame::OnFileOpen, this, ID_OPENFILE);
Bind(wxEVT_MENU, &PlayerFrame::OnFilesOpen, this, ID_OPEN_FILES);
Bind(wxEVT_MEDIA_LOADED, &PlayerFrame::OnMediaLoaded, this, ID_MEDIA_CTRL);
Bind(wxEVT_MEDIA_FINISHED, &PlayerFrame::OnMediaFinished, this, ID_MEDIA_CTRL);

// In MediaControls constructor
Bind(wxEVT_BUTTON, &MediaControls::OnPlay, this, btn_play->GetId());
Bind(wxEVT_SLIDER, &MediaControls::OnVolumeChange, this, slider_volume->GetId());
Bind(wxEVT_TIMER, &MediaControls::OnUpdateTimer, this, update_timer->GetId());
```

### Cross-Component Communication

```
PlayerFrame ←→ MediaControls
    │              │
    │              ├── SetPlaylist(playlist)
    │              └── SetStatusBar(status_bar)
    │
    ├── playlist ←─┴── Navigation events
    └── status_bar ←─── Update events
```

## Media Pipeline

### wxMediaCtrl Integration

```cpp
// Media control creation with error handling
media_ctrl = new wxMediaCtrl();
if (!media_ctrl->Create(video_canvas_pane, ID_MEDIA_CTRL)) {
    wxLogError("Failed to create media control");
    return;
}
```

### GStreamer Backend Configuration

**Environment Setup:**
```cpp
// In WanjPlayer::OnInit()
wxSetEnv("GST_GL_DISABLED", "1");           // Disable OpenGL
wxSetEnv("LIBGL_ALWAYS_SOFTWARE", "1");     // Force software rendering
wxSetEnv("GST_VIDEO_SINK", "ximagesink");   // Use X11 video sink
```

**Plugin Management:**
```bash
# Disabled problematic plugins
export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload"
```

### Media State Management

```cpp
// State tracking in MediaControls
class MediaControls {
private:
    wxFileOffset media_duration;    // Total file duration
    wxFileOffset media_position;    // Current playback position
    
    void OnUpdateTimer(wxTimerEvent& event) {
        // Update duration if changed
        wxFileOffset current_duration = _pmedia_ctrl->Length();
        if (current_duration != media_duration) {
            media_duration = current_duration;
            slider_playback_position->SetMax(media_duration / 100);
        }
        
        // Update position
        media_position = _pmedia_ctrl->Tell();
        UpdatePositionSlider();
        UpdateTimeDisplay();
    }
};
```

## Build System

### CMake Configuration

**File:** `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.30.3)
project(WanjPlayer VERSION 0.1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project directories
set(SOURCE_DIR ${PROJECT_ROOT}/src)
set(INCLUDE_DIR ${PROJECT_ROOT}/include)
set(BIN_DIR ${PROJECT_ROOT}/build)

# wxWidgets integration
find_package(wxWidgets REQUIRED COMPONENTS base core media)
include(${wxWidgets_USE_FILE})

# Executable definition
add_executable(WanjPlayer
    ${SOURCE_DIR}/wanjplayer.cpp
    ${SOURCE_DIR}/menubar.cpp
    ${SOURCE_DIR}/statusbar.cpp
    ${SOURCE_DIR}/about.cpp
    ${SOURCE_DIR}/license_dialogbx.cpp
    ${SOURCE_DIR}/media_ctrls.cpp
    ${SOURCE_DIR}/playlist.cpp
    ${SOURCE_DIR}/media_events.cpp
)

# Linking
target_link_libraries(WanjPlayer ${wxWidgets_LIBRARIES})

# Output directory
set_target_properties(WanjPlayer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
```

### Enhanced Build Script

**File:** `build.sh`

**Key Features:**
- Environment detection (Wayland/X11/Hybrid)
- Dependency verification
- Automatic wrapper script generation
- Development tool integration

**Build Workflow:**
```bash
./build.sh rebuild
├── detect_environment()
├── configure_build_environment()
├── check_dependencies()
├── configure_project()  # CMake configuration
├── build_project()     # Compilation
├── create_build_runner()  # Wrapper script
├── create_build_readme()  # Documentation
└── update_compile_commands()  # Language server support
```

## Environment Management

### Display System Detection

```cpp
// Environment detection logic (in shell scripts)
SESSION_TYPE="${XDG_SESSION_TYPE:-unknown}"
WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-}"
X11_DISPLAY="${DISPLAY:-}"

if [ -n "$WAYLAND_DISPLAY" ] && [ "$SESSION_TYPE" = "wayland" ]; then
    DETECTED_SYSTEM="wayland"
elif [ -n "$X11_DISPLAY" ] && [ "$SESSION_TYPE" = "x11" ]; then
    DETECTED_SYSTEM="x11"
elif [ -n "$WAYLAND_DISPLAY" ] && [ -n "$X11_DISPLAY" ]; then
    DETECTED_SYSTEM="hybrid"
else
    DETECTED_SYSTEM="unknown"
fi
```

### Compatibility Configuration

**Wayland Systems:**
```bash
export GDK_BACKEND=x11              # Force X11 backend
export QT_QPA_PLATFORM=xcb          # Use XCB platform
export GST_GL_DISABLED=1            # Disable OpenGL
export LIBGL_ALWAYS_SOFTWARE=1      # Software rendering
export GST_VIDEO_SINK=ximagesink    # X11 video sink
```

**X11 Systems:**
```bash
export GST_GL_DISABLED=1            # Conservative OpenGL handling
export GST_VIDEO_SINK=ximagesink    # Native X11 video sink
```

## API Reference

### MediaControls Public Interface

```cpp
class MediaControls : public wxPanel {
public:
    // Constructor
    MediaControls(wxPanel* parent, wxMediaCtrl* media_ctrl);
    
    // Configuration
    void SetPlaylist(Playlist* playlist);        // Enable navigation
    void SetStatusBar(gui::StatusBar* status);   // Enable status updates
    
    // Media state management
    void UpdateDuration();                       // Refresh duration info
    
    // Destructor
    ~MediaControls();
    
private:
    // Internal event handlers
    void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnPrevious(wxCommandEvent& event);
    void OnVolumeChange(wxCommandEvent& event);
    void OnPositionSliderChange(wxCommandEvent& event);
    void OnUpdateTimer(wxTimerEvent& event);
    
    // Utility methods
    wxString FormatTime(wxFileOffset milliseconds);
    void UpdateTimeDisplay();
    void UpdatePositionSlider();
};
```

### Playlist Public Interface

```cpp
class Playlist : public wxListBox {
public:
    // Constructor
    Playlist(wxWindow* parent, wxWindowID id);
    
    // Queue management
    void add_item(const wxString& path);         // Add file to queue
    void clear_play_queue();                     // Remove all items
    
    // Item access
    wxString get_item(size_t index) const;       // Get path by index
    size_t get_count() const;                    // Total items
    bool is_empty() const;                       // Check if empty
    
    // Navigation
    void play_next_item_in_queue();              // Advance to next
    void play_previous_item_in_queue();          // Go to previous
    void play_item_at_index(size_t index);       // Jump to specific item
    
    // Current state
    void set_current_index(size_t index);        // Set active item
    size_t get_current_index() const;            // Get active index
    wxString get_current_item() const;           // Get current path
    
    // Navigation queries
    bool has_next() const;                       // Can advance?
    bool has_previous() const;                   // Can go back?
    
    // Media control integration
    wxMediaCtrl* GetMediaCtrl();                 // Get associated control
    void SetMediaCtrl(wxMediaCtrl* ctrl);        // Associate control
    
private:
    // Internal data
    std::vector<wxString> play_queue;            // File paths
    size_t current_index;                        // Active item
    wxMediaCtrl* media_ctrl_ref;                 // Media control reference
    
    // Event handlers
    void OnKeyDown(wxKeyEvent& event);           // Keyboard navigation
    void OnDoubleClick(wxCommandEvent& event);   // Item activation
    void OnListSelection(wxCommandEvent& event); // Selection changes
    
    // Visual feedback
    void highlight_current_track();              // Update UI selection
};
```

### StatusBar Public Interface

```cpp
class StatusBar {
public:
    // Constructor
    StatusBar(wxFrame* parent);
    
    // Initialization
    void create_statusbar();                     // Create status fields
    
    // Information updates
    void update_file_info(const wxString& filename, 
                         wxFileOffset duration_ms); // File and duration
    void update_playback_info(const wxString& status); // Play state
    void set_duration_display(const wxString& time);   // Time counter
    void set_system_message(const wxString& msg);      // System info
    
    // Maintenance
    void clear_all();                            // Reset all fields
    
    // Destructor
    ~StatusBar();
    
private:
    wxFrame* _parent;                            // Parent window
    wxString format_duration(wxFileOffset ms);   // Time formatting
};
```

## Extension Guide

### Adding New Media Controls

1. **Declare button in MediaControls:**
```cpp
// In include/media_ctrls.hpp
private:
    wxButton* btn_shuffle;  // New shuffle button
```

2. **Create button in constructor:**
```cpp
// In src/media_ctrls.cpp constructor
btn_shuffle = new wxButton(this, wxID_ANY, "Shuffle");
```

3. **Add to layout:**
```cpp
// In button sizer
button_sizer->Add(btn_shuffle, 0, wxALL, 2);
```

4. **Bind event:**
```cpp
// In constructor
Bind(wxEVT_BUTTON, &MediaControls::OnShuffle, this, btn_shuffle->GetId());
```

5. **Implement handler:**
```cpp
// In MediaControls class
void MediaControls::OnShuffle(wxCommandEvent& event) {
    if (playlist) {
        playlist->toggle_shuffle();
    }
}
```

### Adding New Menu Items

1. **Define ID:**
```cpp
// In include/wanjplayer.hpp enum
enum {
    ID_NEW_FEATURE = wxID_HIGHEST + 100,  // Avoid conflicts
    // ... existing IDs
};
```

2. **Create menu item:**
```cpp
// In src/menubar.cpp
playback_menu->Append(ID_NEW_FEATURE, "&New Feature\tCtrl+N", 
                      "Description of new feature");
```

3. **Bind event in PlayerFrame:**
```cpp
// In PlayerFrame constructor
Bind(wxEVT_MENU, &PlayerFrame::OnNewFeature, this, ID_NEW_FEATURE);
```

4. **Implement handler:**
```cpp
// In PlayerFrame
void PlayerFrame::OnNewFeature(wxCommandEvent& event) {
    // Implementation here
}
```

### Adding New File Format Support

File format support is handled by wxMediaCtrl/GStreamer. To add support:

1. **Install GStreamer plugins:**
```bash
sudo apt install gstreamer1.0-plugins-bad  # For additional formats
```

2. **Check format support:**
```cpp
// In code, you can query supported formats
wxFileDialog dialog(this, "Open Media File", "", "",
    "Media files (*.mp3;*.mp4;*.avi;*.mkv;*.flac)|*.mp3;*.mp4;*.avi;*.mkv;*.flac",
    wxFD_OPEN | wxFD_FILE_MUST_EXIST);
```

3. **Add format to file dialogs:**
```cpp
// Update file filter strings in PlayerFrame::OnFilesOpen()
```

### Creating Custom UI Panels

1. **Create header file:**
```cpp
// include/my_panel.hpp
#ifndef __MY_PANEL_HPP
#define __MY_PANEL_HPP

#include "wanjplayer.hpp"

namespace gui {
class MyPanel : public wxPanel {
public:
    MyPanel(wxWindow* parent);
    
private:
    void OnCustomEvent(wxCommandEvent& event);
};
}

#endif
```

2. **Implement functionality:**
```cpp
// src/my_panel.cpp
#include "my_panel.hpp"

gui::MyPanel::MyPanel(wxWindow* parent) 
    : wxPanel(parent, wxID_ANY) {
    // Setup UI
}
```

3. **Integrate with main window:**
```cpp
// In PlayerFrame constructor
my_custom_panel = new gui::MyPanel(this);
// Add to layout sizers
```

## Debugging Guide

### Common Debug Scenarios

#### 1. Media Won't Load

**Symptoms:** Files selected but don't play

**Debug Steps:**
```cpp
// Add to PlayerFrame::OnMediaLoaded()
void PlayerFrame::OnMediaLoaded(wxMediaEvent& event) {
    wxLogMessage("Media loaded successfully");
    wxLogMessage("Duration: %lld ms", media_ctrl->Length());
    wxLogMessage("State: %d", media_ctrl->GetState());
    
    // Your existing code...
}
```

**Check GStreamer:**
```bash
# Run with GStreamer debug
GST_DEBUG=3 build/WanjPlayer

# Test file directly
gst-play-1.0 /path/to/media/file.mp3
```

#### 2. Seek Slider Not Working

**Debug in MediaControls:**
```cpp
void MediaControls::OnPositionSliderChange(wxCommandEvent& event) {
    int slider_value = slider_playback_position->GetValue();
    wxFileOffset new_position = static_cast<wxFileOffset>(slider_value) * 100;
    
    wxLogMessage("Seek: slider=%d, position=%lld, duration=%lld", 
                 slider_value, new_position, media_duration);
    
    if (_pmedia_ctrl && media_duration > 0) {
        if (_pmedia_ctrl->Seek(new_position)) {
            wxLogMessage("Seek successful");
        } else {
            wxLogMessage("Seek failed");
        }
    }
}
```

#### 3. UI Layout Issues

**Debug sizers:**
```cpp
// In PlayerFrame constructor, after layout setup
GetSizer()->SetSizeHints(this);  // Ensure proper sizing
Layout();  // Force layout recalculation

// Check minimum sizes
wxLogMessage("Min size: %dx%d", GetMinSize().x, GetMinSize().y);
```

#### 4. Environment Issues

**Check environment variables:**
```cpp
// In WanjPlayer::OnInit()
wxString gdk_backend;
wxGetEnv("GDK_BACKEND", &gdk_backend);
wxLogMessage("GDK_BACKEND: %s", gdk_backend);

wxString gst_debug;
wxGetEnv("GST_DEBUG", &gst_debug);
wxLogMessage("GST_DEBUG: %s", gst_debug);
```

### Debug Build Configuration

```bash
# Build with debug symbols and sanitizers
cmake -S . -B debug -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="-g -O0 -fsanitize=address -fsanitize=undefined"
cmake --build debug
```

### Memory Debugging

```bash
# Run with AddressSanitizer
export ASAN_OPTIONS="abort_on_error=1:halt_on_error=1"
./debug/WanjPlayer

# Run with Valgrind
valgrind --tool=memcheck --leak-check=full build/WanjPlayer
```

### GDB Debugging Session

```bash
gdb build/WanjPlayer
(gdb) set args
(gdb) break PlayerFrame::PlayerFrame
(gdb) break MediaControls::OnPlay
(gdb) run
(gdb) bt  # Show backtrace
(gdb) info locals  # Show local variables
(gdb) continue
```

## Performance Considerations

### Timer Optimization

**Current Implementation:**
```cpp
// 100ms update interval in MediaControls
update_timer->Start(100);  // 10 times per second
```

**Considerations:**
- 100ms provides smooth updates without excessive CPU usage
- For battery-powered devices, consider 200-500ms intervals
- Pause timer when media is stopped/paused

### Memory Management

**wxWidgets Object Lifecycle:**
```cpp
// wxWidgets uses automatic cleanup for child windows
// Manual cleanup only needed for:
// - Timers (done in destructor)
// - Custom allocated objects
// - Event bindings (automatic)

MediaControls::~MediaControls() {
    if (update_timer) {
        update_timer->Stop();
        delete update_timer;
    }
}
```

### Threading Considerations

**Current Architecture:**
- Single-threaded GUI design
- GStreamer handles media I/O in background threads
- Timer events processed on main thread
- No manual thread management required

**Future Threading Improvements:**
```cpp
// Example: Background file analysis
class MediaAnalyzer : public wxThread {
public:
    MediaAnalyzer(const wxString& file) : wxThread(wxTHREAD_DETACHED) {}
    
protected:
    virtual ExitCode Entry() override {
        // Analyze media file properties
        // Post results to main thread via events
        return 0;
    }
};
```

## Conclusion

WanjPlayer v0.1.0 provides a solid foundation for cross-platform media playback with modern C++ architecture and robust environment compatibility. The modular design allows for easy extension and maintenance while maintaining performance and stability.

### Key Architectural Strengths
- **Clean separation of concerns** between UI, media control, and playlist management
- **Event-driven architecture** enabling loose coupling between components
- **Environment-aware design** handling Wayland/X11 compatibility automatically
- **Extensible structure** supporting future feature additions
- **Modern C++ practices** with smart pointers and RAII

### Development Best Practices
- Use the enhanced build system for consistent environment setup
- Follow the established event binding patterns
- Maintain the namespace organization
- Write unit tests for new components
- Update documentation for API changes

This documentation serves as a complete reference for understanding, extending, and debugging WanjPlayer. For specific implementation questions, refer to the source code examples and API reference sections above.