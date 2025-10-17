# [![Codacy Badge](https://app.codacy.com/project/badge/Grade/51580f11c9ff43b0903a98a2ae77d323)](https://app.codacy.com/gh/Magpiny/wanjplayer/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade) [![C++ Build CI](https://github.com/Magpiny/wanjplayer/actions/workflows/c-cpp.yml/badge.svg?branch=main)](https://github.com/Magpiny/wanjplayer/actions/workflows/c-cpp.yml)

# wanjplayer
Media player in C++

## Dependencies
1. wxWidgets 3.2.6
2. libvlcpp (Optional)

## PREREQUISITES
* wxWidgets 3.2.6
* CMake 3.30.3
* Latest versions of gcc or clang
* Make build system

## Usage:

#### Create a new directory (Assuming that you're on a unix system)
```bash
 mkdir wanjplayer
```
#### Move into the newly created directory
```bash
  cd wanjplayer
```
##### Clone the repo
```bash
 git clone https://github.com/Magpiny/wanjplayer.git
```

```bash
  cd wanjplayer
```
#### Assuming you have the lates version of wxWidgets and cmake installed in your system. Build the app
```bash
   cmake -S . build
```
#### cd into the build directory and build the app
```bash
  cd build
```
```bash
  cmake --build .
```

#### Run the app
```bash
  ./wanjplayer
```

The main launcher automatically detects your display system (Wayland/X11) and configures the optimal video backend for seamless playback.

#### Alternative launch methods
```bash
  ./build/WanjPlayer          # Direct executable (basic video support)
  ./launch.sh                 # Simplified launcher with auto-config
  ./run_safe_video.sh         # Legacy script with multiple fallbacks
```

#### Testing Features
```bash
  ./test_improvements.sh      # Test playlist and UI improvements
  ./test_status_bar_timer.sh  # Test real-time status bar timer updates
  ./wanjplayer --info         # Show system info and video compatibility
  ./wanjplayer --debug        # Run with debug output
```

#### Exit the app
 Press exit/quit from the app (The recommended way)
 Alternatively press CTRL+C / CMD+C from the terminal

## Video Playback - Now Mainstream! 🎬

**FIXED**: Video files now play correctly on all systems without crashes!

### Automatic Video Configuration
- **Smart Detection**: Automatically detects Wayland vs X11 environments
- **Seamless Setup**: No manual configuration needed
- **Universal Compatibility**: Works on any Linux distribution
- **Crash Prevention**: Eliminates GStreamer GL segmentation faults

### Launch Options
```bash
./wanjplayer               # Universal launcher (recommended)
./wanjplayer --safe        # Maximum compatibility mode
./wanjplayer --debug       # Troubleshooting mode
./wanjplayer --info        # Show system configuration
```

## Enhanced Features & Improvements

### Enhanced Playlist System
- **End-to-End Playback**: Files play automatically one after another
- **Clickable Playlist Items**: Double-click any track to play it immediately
- **Smart Navigation**: Working Next/Previous buttons with proper bounds checking
- **Current Track Highlighting**: Visual indication of currently playing track
- **Keyboard Controls**: 
  - Space bar to play selected track
  - Delete/Backspace to remove tracks from playlist
- **Auto-advance**: Automatically plays next track when current finishes

### Advanced Media Controls
- **Real-time Duration Display**: Shows current time / total duration (HH:MM:SS format)
- **Functional Seek Slider**: Click anywhere to jump to that position in the track
- **Live Position Updates**: Updates every second during playback
- **Enhanced Button Layout**: Redesigned control panel with better organization
- **Volume Control**: Smooth volume adjustment slider
- **Comprehensive Playback States**: Play, Pause, Stop, Previous, Next all fully functional

### Enhanced Status Bar (4 Columns)
- **Column 0**: System messages and logging information
- **Column 1**: Current filename (auto-truncated for long names)
- **Column 2**: **LIVE TIMER**: Current time / Total duration (updates every second!)
- **Column 3**: Playback status (Playing/Paused/Stopped/etc.)
- **Real-time Updates**: Duration counter updates every second during playback

### Collapsible Playlist
- **Fullscreen Video Mode**: Hide playlist for distraction-free video viewing
- **Multiple Toggle Methods**:
  - Click "Hide/Show Playlist" button
  - Press F9 keyboard shortcut
  - Use View → Toggle Playlist menu option
- **State Persistence**: Remembers playlist visibility preference
- **Status Integration**: Shows fullscreen mode status in status bar

### Robust Error Handling
- **Segfault Prevention**: Comprehensive safety checks prevent crashes
- **Graceful Navigation**: Safe handling of playlist boundaries
- **Error Logging**: Detailed error messages for troubleshooting
- **Exception Handling**: Try-catch blocks around critical operations
- **Null Pointer Protection**: All media operations check for valid objects

## User Interface Guide

### Loading Multiple Files
1. **File → Open Files** (Ctrl+Shift+O) - Load multiple media files
2. Files appear in playlist
3. Playback starts automatically with first track
4. Status bar shows file information

### Playlist Navigation
- **Double-click** playlist items to play specific tracks
- **Next/Previous buttons** for sequential navigation
- **Current track** highlighted in playlist
- **Auto-advance** when tracks finish
- **Keyboard shortcuts** for quick control

### Fullscreen Video Experience
- **Hide Playlist**: Use button, F9, or View menu
- **Show Playlist**: Same methods to restore
- **Status indication**: Shows fullscreen mode in status bar
- **Seamless toggling**: Instant response without interruption

### Status Bar Information
- **System Messages**: Real-time logging and system status
- **File Information**: Current track name and live duration timer
- **Live Duration Counter**: Shows current position / total duration (e.g., "02:15 / 04:30")
- **Timer Updates**: Counter increments every second during playback
- **Playback Status**: Current player state
- **Visual Feedback**: Updates reflect all user actions

## Troubleshooting

### Video Playback Issues on Wayland
If you experience crashes, segmentation faults, or GStreamer-GL-CRITICAL errors when playing video files (while audio works fine), this is due to GStreamer OpenGL conflicts with Wayland. **Solution:**

1. **Use the safe video script (Recommended)**:
   ```bash
   ./run_safe_video.sh
   ```

2. **Alternative manual approaches**:
   
   **Option A: Force X11 backend (most reliable)**:
   ```bash
   export GDK_BACKEND=x11
   export GST_GL_DISABLED=1
   export LIBGL_ALWAYS_SOFTWARE=1
   ./WanjPlayer
   ```
   
   **Option B: Clean GStreamer registry**:
   ```bash
   ./clean_gst_registry.sh
   ```

### Understanding the Issue
The problem occurs because:
- GStreamer tries to use OpenGL for video rendering
- On Wayland, this conflicts with the compositor's graphics handling
- wxWidgets with GStreamer backend doesn't handle this conflict gracefully

The safe video script automatically:
1. Tests available video sinks (ximagesink, waylandsink)
2. Configures appropriate environment variables
3. Falls back to software rendering if needed

### Issues Resolved ✅

1. **Video Playback Crashes (FIXED)**:
   - Issue: Segmentation faults and GStreamer GL conflicts on Wayland
   - Solution: Automatic detection and configuration of optimal video backend
   - Status: Video now works seamlessly on all Linux systems

2. **Previous/Next Button Crashes (FIXED)**:
   - Issue: Segmentation faults when navigating playlist
   - Solution: Comprehensive error checking and bounds validation
   - Status: All navigation operations now safe and logged

3. **Missing File Information (FIXED)**:
   - Issue: No file information or playback duration displayed
   - Solution: Enhanced 4-column status bar with real-time updates
   - Status: Full file info, duration (HH:MM:SS), and status tracking

4. **Playlist Navigation Issues (FIXED)**:
   - Issue: Playlist items not clickable, no auto-advance
   - Solution: Complete playlist rewrite with proper track management
   - Status: Full playlist functionality working

5. **No Fullscreen Video Mode (FIXED)**:
   - Issue: Playlist always visible, blocking video viewing
   - Solution: Collapsible playlist with multiple toggle methods
   - Status: F9, button, and menu toggle all working

### Requirements for Video Playback
- GStreamer 1.0 or later
- GStreamer plugins: `gst-plugins-base`, `gst-plugins-good`, `gst-plugins-bad`
- For Wayland systems: XWayland support (usually pre-installed)
- Working video sink: either `ximagesink` or `waylandsink`

### Testing Your Installation
```bash
# Test all features with automatic video configuration
./wanjplayer --info

# Test playlist functionality
./test_improvements.sh

# Test real-time status bar timer
./test_status_bar_timer.sh

# Legacy video testing (now unnecessary)
./run_safe_video.sh

# Clean GStreamer registry if needed
./clean_gst_registry.sh
```

## Implementation Details

### Files Modified for Improvements
- **Playlist System**: `src/playlist.cpp`, `include/playlist.hpp`
- **Media Controls**: `src/media_ctrls.cpp`, `include/media_ctrls.hpp` 
- **Status Bar**: `src/statusbar.cpp`, `include/statusbar.hpp`
- **Main Application**: `src/wanjplayer.cpp`, `include/wanjplayer.hpp`
- **Event Handling**: `src/media_events.cpp`
- **Menu System**: `src/menubar.cpp`
- **Widget Headers**: `include/widgets.hpp`

### Key Technical Improvements
- **Automatic Video Backend**: Smart detection and configuration for any Linux system
- **Wayland Compatibility**: Seamless video playback on modern Linux desktops
- **Memory Safety**: Comprehensive pointer validation and exception handling
- **Event System**: Robust media event handling with proper error recovery
- **Timer Management**: Real-time updates with proper cleanup
- **UI Responsiveness**: Non-blocking operations and smooth interactions
- **State Management**: Proper tracking of playlist position and playback state

### Performance Optimizations
- **Real-time Updates**: 1-second timer updates for live duration counter
- **Smart Refreshing**: Only update UI elements when values change
- **Error Recovery**: Graceful handling of failed operations
- **Resource Management**: Proper cleanup of timers and media resources

## Extend the app and share with your friends
## Submit a PR or feature request


