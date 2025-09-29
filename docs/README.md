# WanjPlayer v0.1.0 Documentation Index

**Project:** WanjPlayer - Cross-Platform Media Player  
**Version:** 0.1.0  
**Author:** Wanjare Samuel  
**License:** MIT  
**Repository:** [WanjPlayer GitHub](https://github.com/wanjaresamuel/wanjplayer)

## 📚 Documentation Overview

This documentation suite provides comprehensive technical and development information for WanjPlayer, a modern C++23 media player built with wxWidgets and GStreamer.

## 📖 Document Structure

### Core Documentation

| Document | Description | Audience |
|----------|-------------|----------|
| **[documentation.md](documentation.md)** | Complete technical documentation with system architecture, API reference, and extension guides | Developers, Contributors |
| **[DEVELOPMENT.md](DEVELOPMENT.md)** | Comprehensive development stages, technical decisions, and project evolution | Developers, Maintainers |

### Problem-Specific Documentation

| Document | Description | Focus |
|----------|-------------|-------|
| **[../SEGFAULT_SOLUTION.md](../SEGFAULT_SOLUTION.md)** | Complete analysis and resolution of segmentation fault issues | Debugging, Environment Setup |
| **[../LOGGING_AND_SEEK_FIXES.md](../LOGGING_AND_SEEK_FIXES.md)** | Logging cleanup and seek slider precision improvements | User Experience, Quality |
| **[../BUILD_SYSTEM_ENHANCEMENT.md](../BUILD_SYSTEM_ENHANCEMENT.md)** | Enhanced build system with environment detection | Build Process, Developer Experience |

## 🚀 Quick Start Guide

### For Users
```bash
# Build and run
./build.sh rebuild
./wanjplayer

# Alternative execution methods
./build.sh run
cd build && ./run_wanjplayer.sh
```

### For Developers
```bash
# Complete setup
./build.sh deps          # Install dependencies
./build.sh rebuild       # Build with environment setup
./build.sh info          # Verify system compatibility

# Development workflow  
./build.sh format        # Format code
./build.sh check         # Static analysis
./build.sh run           # Test changes
```

## 🏗️ Architecture Overview

WanjPlayer follows a modern component-based architecture:

```
┌─────────────────────────────────────────────┐
│              WanjPlayer App                 │
├─────────────────────────────────────────────┤
│  PlayerFrame (Main Window)                  │
│  ├── gui::Menubar                          │
│  ├── gui::StatusBar                        │
│  ├── gui::player::MediaControls            │
│  ├── gui::player::Playlist                 │
│  └── wxMediaCtrl (GStreamer Backend)       │
├─────────────────────────────────────────────┤
│       Environment Compatibility Layer      │
│    (Wayland/X11 Detection & Setup)         │
└─────────────────────────────────────────────┘
```

## 🔧 Key Features

### Current (v0.1.0)
- ✅ Cross-platform media playback (Linux Wayland/X11)
- ✅ Advanced playlist management with navigation
- ✅ High-precision seeking (0.1-second resolution)  
- ✅ Smart environment detection and compatibility
- ✅ Enhanced build system with wrapper generation
- ✅ Professional UI with keyboard shortcuts

### Supported Formats
- **Audio:** MP3, FLAC, WAV, OGG, AAC, M4A
- **Video:** MP4, MKV, AVI, MOV, WEBM

## 📋 Technical Specifications

| Component | Version | Purpose |
|-----------|---------|---------|
| **Language** | C++23 | Modern features and performance |
| **GUI Framework** | wxWidgets 3.2.6+ | Cross-platform native interface |
| **Media Backend** | GStreamer | Comprehensive format support |
| **Build System** | CMake 3.30.3+ | Cross-platform compilation |
| **Code Quality** | clangd, clang-format, clang-tidy | Modern development tools |

## 🚨 Important Usage Notes

### ⚠️ Direct Execution Warning
**DO NOT** run `./build/WanjPlayer` directly - it will crash with segmentation fault due to missing environment setup.

### ✅ Correct Usage Methods
```bash
# Recommended methods (all handle environment automatically)
./wanjplayer                    # Main launcher script
./build.sh run                  # Build system execution
./build/run_wanjplayer.sh       # Build directory wrapper
```

## 🛠️ Environment Compatibility

WanjPlayer automatically detects and configures for:

| System Type | Configuration | Status |
|-------------|---------------|--------|
| **Pure Wayland** | XWayland backend, software rendering | ✅ Supported |
| **Pure X11** | Native X11 backend | ✅ Supported |
| **Hybrid** | Wayland-compatible settings | ✅ Supported |
| **Unknown** | Safe fallback configuration | ✅ Supported |

### Environment Variables Set Automatically
```bash
# Wayland Compatibility
export GDK_BACKEND=x11
export GST_GL_DISABLED=1
export LIBGL_ALWAYS_SOFTWARE=1
export GST_VIDEO_SINK=ximagesink

# Debug Mode (when --debug flag used)
export GST_DEBUG=3
```

## 📚 Documentation Navigation

### Getting Started
1. **[Quick Start](../QUICK_START.md)** - Basic usage and setup
2. **[Build System](../BUILD_SYSTEM_ENHANCEMENT.md)** - Understanding the enhanced build process

### Development
1. **[DEVELOPMENT.md](DEVELOPMENT.md)** - Complete development history and decisions
2. **[documentation.md](documentation.md)** - Technical architecture and API reference
3. **[Extension Guide](documentation.md#extension-guide)** - Adding new features

### Troubleshooting
1. **[Segfault Issues](../SEGFAULT_SOLUTION.md)** - Environment-related crashes
2. **[Quality Issues](../LOGGING_AND_SEEK_FIXES.md)** - Logging and precision problems
3. **[Build Issues](documentation.md#debugging-guide)** - Compilation and dependency problems

## 🎯 Use Cases

### End Users
- **Media Playback:** Audio and video file playback with professional controls
- **Playlist Management:** Advanced queue management with navigation
- **Cross-Platform:** Seamless operation on modern Linux systems

### Developers
- **Learning Resource:** Modern C++ GUI development with wxWidgets
- **Architecture Reference:** Component-based design patterns
- **Extension Base:** Foundation for custom media player features

### System Administrators
- **Deployment:** Understanding environment requirements and setup
- **Compatibility:** Cross-system deployment considerations
- **Troubleshooting:** Comprehensive problem resolution guides

## 🔄 Version Information

### Current Release: v0.1.0
- **Release Date:** September 2024
- **Stability:** Production-ready core functionality
- **Testing:** Verified across Wayland/X11 environments
- **Documentation:** Complete technical and development docs

### Development Metrics
- **Development Time:** 30 days
- **Lines of Code:** ~4,000 (C++ + Shell)
- **Documentation:** 8,000+ lines across 8 documents
- **Supported Environments:** 4 (Wayland, X11, Hybrid, Unknown)

## 🤝 Contributing

### For Contributors
1. Read **[DEVELOPMENT.md](DEVELOPMENT.md)** for development history and decisions
2. Study **[documentation.md](documentation.md)** for architecture understanding
3. Follow established patterns and code style
4. Test across different Linux environments
5. Update documentation for any architectural changes

### For Users
- Report issues with complete environment information
- Request features with detailed use cases
- Share experience across different Linux distributions

## 📞 Support

### Documentation-First Support
1. **Check Documentation:** Most questions answered in comprehensive docs
2. **System Analysis:** Use `./build.sh info` for environment diagnosis  
3. **Debug Mode:** Use `--debug` flags for verbose troubleshooting
4. **Issue Templates:** Follow structured issue reporting format

### Getting Help
```bash
# System diagnosis
./build.sh info                    # Build system analysis
./wanjplayer --info                # Runtime environment check
./build/run_wanjplayer.sh --help   # All available options
```

## 🏆 Project Achievements

### Technical Excellence
- **Zero Critical Bugs:** Stable v0.1.0 release
- **Cross-Environment Compatibility:** Works on all tested Linux systems
- **Professional User Experience:** High-precision controls and clean interface
- **Modern Architecture:** C++23 with contemporary development practices

### Development Innovation
- **Smart Build System:** Automatic environment detection and setup
- **Comprehensive Documentation:** Technical and historical documentation
- **Quality Focus:** Extensive testing and precision improvements
- **Developer Experience:** Enhanced tooling and automation

## 📈 Future Roadmap

### Short-Term (v0.2.0)
- Enhanced playlist features (drag-drop, save/load)
- Expanded keyboard shortcuts
- Additional media format support

### Medium-Term (v0.3.0-v0.5.0)
- Equalizer and advanced audio controls
- Theme system and visual customization
- Performance optimizations

### Long-Term (v1.0.0+)
- Plugin architecture
- Network streaming capabilities
- Cross-platform expansion (Windows, macOS)

---

**Last Updated:** September 2024  
**Documentation Version:** v0.1.0  
**Maintainer:** Wanjare Samuel