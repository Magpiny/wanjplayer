# WanjPlayer Quick Start Guide

Welcome to WanjPlayer! This guide will get you up and running in minutes.

## 🚀 Installation & First Run

### 1. Build WanjPlayer
```bash
git clone https://github.com/Magpiny/wanjplayer.git
cd wanjplayer
cmake -B build -S .
cmake --build build
```

### 2. Launch WanjPlayer
```bash
./wanjplayer
```
**That's it!** WanjPlayer automatically detects your system and configures optimal video settings.

## 🎵 Basic Usage

### Load Your Media Files
1. **Single File**: `File → Open File` (Ctrl+O)
2. **Multiple Files**: `File → Open Files` (Ctrl+Shift+O)
3. **Drag & Drop**: *(Coming soon)*

### Control Playback
- **Play/Pause/Stop**: Use the media control buttons
- **Volume**: Drag the volume slider
- **Seek**: Click anywhere on the position slider
- **Next/Previous**: Navigate through your playlist

### Playlist Management
- **Double-click** any playlist item to play it
- **Space bar** to play selected item
- **Delete key** to remove items from playlist
- **F9** to hide/show playlist (great for fullscreen video!)

## 📺 Video Playback

### Automatic Configuration
WanjPlayer automatically detects and configures:
- ✅ **Wayland** systems (Ubuntu 22+, Fedora 36+, etc.)
- ✅ **X11** systems (traditional Linux desktops)
- ✅ **Hybrid** environments (Wayland + XWayland)

### Supported Formats
**Video**: MP4, MKV, AVI, MOV, WEBM, 3GP  
**Audio**: MP3, WAV, FLAC, OGG, AAC, M4A

### Fullscreen Experience
Press **F9** to hide the playlist for distraction-free video viewing!

## 🎛️ Interface Overview

### Status Bar (Bottom)
```
[System Messages] [Current File] [Duration HH:MM:SS] [Playback Status]
```

### Media Controls
```
[00:00] ████████████████████ [03:45]
[Prev] [Play] [Pause] [Stop] [Next]  Volume: ████
```

### Keyboard Shortcuts
- **F9**: Toggle playlist visibility
- **Space**: Play selected playlist item
- **Delete**: Remove playlist item
- **Ctrl+O**: Open single file
- **Ctrl+Shift+O**: Open multiple files

## 🔧 Troubleshooting

### If Video Doesn't Play
Try these commands to get more information:
```bash
./wanjplayer --info     # Check system configuration
./wanjplayer --debug    # Run with detailed logging
./wanjplayer --safe     # Maximum compatibility mode
```

### Common Solutions
1. **Install GStreamer plugins**:
   ```bash
   # Ubuntu/Debian
   sudo apt install gstreamer1.0-plugins-base gstreamer1.0-plugins-good
   
   # Fedora
   sudo dnf install gstreamer1-plugins-base gstreamer1-plugins-good
   
   # Arch Linux
   sudo pacman -S gst-plugins-base gst-plugins-good
   ```

2. **Check system requirements**:
   - CMake 3.30.3+
   - wxWidgets 3.2.6+
   - GStreamer 1.0+

## 🎯 Pro Tips

### Playlist Power Features
- **Auto-advance**: Tracks play automatically one after another
- **Current track highlighting**: See what's playing at a glance
- **End-to-end playback**: Perfect for albums or video series

### Video Optimization
- WanjPlayer automatically uses the best video backend for your system
- No manual configuration needed - it just works!
- Collapsible playlist provides distraction-free video experience

### Performance
- Real-time duration tracking updates every second
- Efficient memory usage with automatic cleanup
- Smooth seeking with 1000-point precision slider

## 📚 Need More Help?

- **Full Documentation**: See `README.md`
- **Feature Details**: See `PLAYLIST_IMPROVEMENTS.md`
- **Technical Info**: See `IMPROVEMENTS_SUMMARY.md`
- **Test Features**: Run `./test_improvements.sh`

## 🎉 You're Ready!

WanjPlayer is designed to work out of the box. The automatic video configuration means you can focus on enjoying your media instead of fighting with settings.

**Happy listening and watching!** 🎵🎬