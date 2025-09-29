#!/bin/bash

# WanjPlayer Build Script with Environment Detection
# Author: Wanjare Samuel
# Description: Enhanced build script for WanjPlayer with environment setup and display detection

set -e  # Exit on any error

# Script metadata
SCRIPT_NAME="WanjPlayer Build System"
VERSION="2.0.0"
AUTHOR="Wanjare Samuel"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Project directories
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"

# Environment variables
DETECTED_SYSTEM=""
VIDEO_MODE=""

# Function to print colored output
print_status() {
    echo -e "${BLUE}ℹ${NC} $1"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_info() {
    echo -e "${CYAN}[INFO]${NC} $1"
}

print_header() {
    echo -e "${PURPLE}════════════════════════════════════════════════════════════${NC}"
    echo -e "${WHITE}  $SCRIPT_NAME v$VERSION${NC}"
    echo -e "${WHITE}  by $AUTHOR${NC}"
    echo -e "${PURPLE}════════════════════════════════════════════════════════════${NC}"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect system environment
detect_environment() {
    SESSION_TYPE="${XDG_SESSION_TYPE:-unknown}"
    WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-}"
    X11_DISPLAY="${DISPLAY:-}"

    print_status "Detecting display system environment..."

    if [ -n "$WAYLAND_DISPLAY" ] && [ "$SESSION_TYPE" = "wayland" ]; then
        DETECTED_SYSTEM="wayland"
        print_info "Session Type: Wayland (primary)"
    elif [ -n "$X11_DISPLAY" ] && [ "$SESSION_TYPE" = "x11" ]; then
        DETECTED_SYSTEM="x11"
        print_info "Session Type: X11 (native)"
    elif [ -n "$WAYLAND_DISPLAY" ] && [ -n "$X11_DISPLAY" ]; then
        DETECTED_SYSTEM="hybrid"
        print_info "Session Type: Hybrid (Wayland + XWayland available)"
    elif [ -n "$X11_DISPLAY" ]; then
        DETECTED_SYSTEM="x11_only"
        print_info "Session Type: X11 (fallback)"
    else
        DETECTED_SYSTEM="unknown"
        print_warning "Session Type: Unknown (using safe defaults)"
    fi
}

# Function to configure build environment
configure_build_environment() {
    print_status "Configuring build environment for optimal compatibility..."

    case "$DETECTED_SYSTEM" in
        "wayland"|"hybrid")
            print_info "Applying Wayland-compatible build configuration..."

            # Set build environment for Wayland compatibility
            export GDK_BACKEND=x11
            export QT_QPA_PLATFORM=xcb

            # Configure for stable compilation on Wayland
            export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
            export DISPLAY="${X11_DISPLAY:-:0}"

            print_success "Wayland-compatible build environment configured"
            VIDEO_MODE="Wayland-compatible"
            ;;

        "x11"|"x11_only")
            print_info "Applying X11 native build configuration..."

            # Native X11 build settings
            export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)

            print_success "X11 native build environment configured"
            VIDEO_MODE="X11-native"
            ;;

        *)
            print_warning "Unknown environment detected, using conservative build settings..."

            # Most conservative settings for unknown systems
            export CMAKE_BUILD_PARALLEL_LEVEL=1

            print_success "Safe fallback build configuration applied"
            VIDEO_MODE="Safe-fallback"
            ;;
    esac
}

# Function to check build dependencies
check_dependencies() {
    print_status "Checking build dependencies..."

    local missing_deps=()

    # Check essential build tools
    if ! command_exists cmake; then
        missing_deps+=("cmake")
    fi

    if ! command_exists g++; then
        missing_deps+=("build-essential")
    fi

    # Check wxWidgets development files
    if ! pkg-config --exists gtk+-3.0 2>/dev/null; then
        missing_deps+=("libgtk-3-dev")
    fi

    # Check for wxWidgets
    if ! command_exists wx-config; then
        missing_deps+=("libwxgtk3.2-dev")
    fi

    # Check GStreamer development files
    if ! pkg-config --exists gstreamer-1.0 2>/dev/null; then
        missing_deps+=("libgstreamer1.0-dev")
    fi

    if [ ${#missing_deps[@]} -eq 0 ]; then
        print_success "All build dependencies are satisfied"
        return 0
    else
        print_warning "Missing dependencies detected: ${missing_deps[*]}"
        print_info "Run './build.sh deps' to install missing dependencies"
        return 1
    fi
}

# Function to check GStreamer plugins for runtime
check_gstreamer() {
    if ! command_exists gst-inspect-1.0; then
        print_warning "GStreamer tools not found in PATH"
        print_info "Video playback may be limited at runtime"
        return 1
    fi

    # Check for essential video sink
    if gst-inspect-1.0 ximagesink >/dev/null 2>&1; then
        print_success "GStreamer video sink available"
        return 0
    else
        print_warning "GStreamer video sink may not be available"
        print_info "Install gst-plugins-base and gst-plugins-good for video support"
        return 1
    fi
}

# Function to clean build directory
clean_build() {
    print_status "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_success "Build directory cleaned"
    else
        print_warning "Build directory doesn't exist"
    fi
}

# Function to configure the project
configure_project() {
    print_status "Configuring project with CMake..."

    # Create build directory if it doesn't exist
    mkdir -p "$BUILD_DIR"

    # Configure with environment-specific settings
    local cmake_args=(
        -S "$PROJECT_ROOT"
        -B "$BUILD_DIR"
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
        -DCMAKE_BUILD_TYPE=Debug
        -DCMAKE_CXX_STANDARD=23
    )

    # Add environment-specific CMake arguments
    case "$DETECTED_SYSTEM" in
        "wayland"|"hybrid")
            cmake_args+=(-DCMAKE_CXX_FLAGS="-DWAYLAND_COMPATIBLE=1")
            ;;
        "x11"|"x11_only")
            cmake_args+=(-DCMAKE_CXX_FLAGS="-DX11_NATIVE=1")
            ;;
    esac

    cmake "${cmake_args[@]}"
    print_success "Project configured successfully"
}

# Function to build the project
build_project() {
    print_status "Building WanjPlayer..."
    cd "$BUILD_DIR"

    # Use environment-configured parallel level
    cmake --build . --parallel "${CMAKE_BUILD_PARALLEL_LEVEL:-$(nproc)}"

    print_success "WanjPlayer built successfully"

    # Verify executable was created
    if [ -f "$BUILD_DIR/WanjPlayer" ]; then
        print_success "Executable created: $(ls -lh "$BUILD_DIR/WanjPlayer" | awk '{print $5}')"

        # Create build directory runner script
        create_build_runner
    else
        print_error "Executable not found after build"
        return 1
    fi
}

# Function to update compile commands symlink
update_compile_commands() {
    print_status "Updating compile_commands.json symlink..."
    cd "$PROJECT_ROOT"

    # Remove existing symlink/file if it exists
    if [ -L "compile_commands.json" ] || [ -f "compile_commands.json" ]; then
        rm -f compile_commands.json
    fi

    # Create symlink to build directory
    if [ -f "$BUILD_DIR/compile_commands.json" ]; then
        ln -s "build/compile_commands.json" compile_commands.json
        print_success "compile_commands.json symlink updated"
    else
        print_error "compile_commands.json not found in build directory"
        return 1
    fi
}

# Function to create build directory runner script
create_build_runner() {
    print_status "Creating build directory runner script..."

    cat > "$BUILD_DIR/run_wanjplayer.sh" << 'EOF'
#!/bin/bash

# WanjPlayer Build Directory Runner
# Author: Wanjare Samuel
# Description: Environment wrapper for running WanjPlayer directly from build directory

set -e

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Script metadata
SCRIPT_NAME="WanjPlayer Build Directory Runner"
VERSION="1.0.0"
AUTHOR="Wanjare Samuel"

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WANJPLAYER_EXEC="$SCRIPT_DIR/WanjPlayer"

# Function to print colored output
print_status() {
    echo -e "${GREEN}✓${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

print_header() {
    echo -e "${PURPLE}════════════════════════════════════════════════════════════${NC}"
    echo -e "${WHITE}  $SCRIPT_NAME v$VERSION${NC}"
    echo -e "${WHITE}  by $AUTHOR${NC}"
    echo -e "${PURPLE}════════════════════════════════════════════════════════════${NC}"
}

# Function to detect system environment
detect_environment() {
    SESSION_TYPE="${XDG_SESSION_TYPE:-unknown}"
    WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-}"
    X11_DISPLAY="${DISPLAY:-}"

    print_info "Detecting display system environment..."

    if [ -n "$WAYLAND_DISPLAY" ] && [ "$SESSION_TYPE" = "wayland" ]; then
        DETECTED_SYSTEM="wayland"
        print_info "Session Type: Wayland (primary)"
    elif [ -n "$X11_DISPLAY" ] && [ "$SESSION_TYPE" = "x11" ]; then
        DETECTED_SYSTEM="x11"
        print_info "Session Type: X11 (native)"
    elif [ -n "$WAYLAND_DISPLAY" ] && [ -n "$X11_DISPLAY" ]; then
        DETECTED_SYSTEM="hybrid"
        print_info "Session Type: Hybrid (Wayland + XWayland available)"
    elif [ -n "$X11_DISPLAY" ]; then
        DETECTED_SYSTEM="x11_only"
        print_info "Session Type: X11 (fallback)"
    else
        DETECTED_SYSTEM="unknown"
        print_warning "Session Type: Unknown (using safe defaults)"
    fi
}

# Function to configure video backend
configure_video_backend() {
    print_info "Configuring optimal video backend..."

    case "$DETECTED_SYSTEM" in
        "wayland"|"hybrid")
            print_info "Applying Wayland compatibility configuration..."

            # Force X11 backend through XWayland for maximum compatibility
            export GDK_BACKEND=x11
            export QT_QPA_PLATFORM=xcb

            # Disable all OpenGL acceleration to prevent segfaults
            export GST_GL_DISABLED=1
            export LIBGL_ALWAYS_SOFTWARE=1
            export MESA_GL_VERSION_OVERRIDE=2.1

            # Configure GStreamer for stable video playback
            export GST_VIDEO_SINK=ximagesink
            export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload"
            export GST_DEBUG=0
            export GST_REGISTRY_FORK=no

            print_status "Wayland video compatibility mode enabled"
            print_status "Using X11 backend via XWayland"
            print_status "OpenGL acceleration disabled for stability"
            VIDEO_MODE="Wayland-safe"
            ;;

        "x11"|"x11_only")
            print_info "Applying X11 native configuration..."

            # Minimal safe settings for X11
            export GST_GL_DISABLED=1
            export GST_VIDEO_SINK=ximagesink
            export GST_DEBUG=0

            print_status "X11 native video backend configured"
            VIDEO_MODE="X11-native"
            ;;

        *)
            print_warning "Unknown environment detected, using conservative settings..."

            # Most conservative settings for unknown systems
            export GST_GL_DISABLED=1
            export LIBGL_ALWAYS_SOFTWARE=1
            export GST_VIDEO_SINK=ximagesink
            export GST_DEBUG=0

            print_status "Safe fallback configuration applied"
            VIDEO_MODE="Safe-fallback"
            ;;
    esac
}

# Function to show system info
show_system_info() {
    echo
    print_info "Runtime Configuration:"
    echo "  • Display System: $DETECTED_SYSTEM"
    echo "  • Video Mode: $VIDEO_MODE"
    echo "  • Wayland: $([ -n "$WAYLAND_DISPLAY" ] && echo "Available" || echo "Not available")"
    echo "  • X11: $([ -n "$X11_DISPLAY" ] && echo "Available" || echo "Not available")"
    echo "  • Executable: $(basename "$WANJPLAYER_EXEC")"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Options:"
    echo "  -h, --help     Show this help message"
    echo "  -v, --version  Show version information"
    echo "  --info         Show system information and exit"
    echo "  --debug        Run with debug output"
    echo "  --safe         Run with maximum safety settings"
    echo
    echo "This script configures the proper environment for running WanjPlayer"
    echo "directly from the build directory with the same stability as the"
    echo "main launcher script."
}

# Function to show version
show_version() {
    echo "$SCRIPT_NAME $VERSION"
    echo "Build directory environment wrapper"
    echo "Copyright (c) 2024 $AUTHOR"
}

# Main execution function
main() {
    local debug_mode=false
    local safe_mode=false
    local show_info_only=false

    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_usage
                exit 0
                ;;
            -v|--version)
                show_version
                exit 0
                ;;
            --info)
                show_info_only=true
                shift
                ;;
            --debug)
                debug_mode=true
                shift
                ;;
            --safe)
                safe_mode=true
                shift
                ;;
            *)
                print_warning "Unknown option: $1"
                print_info "Use --help for usage information"
                exit 1
                ;;
        esac
    done

    # Show header
    print_header
    echo

    # Check if executable exists
    if [ ! -f "$WANJPLAYER_EXEC" ]; then
        print_warning "WanjPlayer executable not found: $WANJPLAYER_EXEC"
        print_info "Please build WanjPlayer first using: ../build.sh build"
        exit 1
    fi

    # Detect environment
    detect_environment

    # Configure video backend
    configure_video_backend

    # Apply additional settings based on flags
    if [ "$debug_mode" = true ]; then
        export GST_DEBUG=3
        print_status "Debug mode enabled"
    fi

    if [ "$safe_mode" = true ]; then
        export LIBGL_ALWAYS_SOFTWARE=1
        export GST_GL_DISABLED=1
        print_status "Maximum safety mode enabled"
    fi

    # Show system info
    show_system_info

    if [ "$show_info_only" = true ]; then
        echo
        print_info "Environment configured - use without --info to run WanjPlayer"
        exit 0
    fi

    echo
    print_info "Starting WanjPlayer from build directory..."
    print_info "Press Ctrl+C to exit"
    print_info "Use F9 to toggle playlist for fullscreen video"
    echo

    # Launch WanjPlayer
    exec "$WANJPLAYER_EXEC"
}

# Cleanup function for graceful exit
cleanup() {
    echo
    print_info "WanjPlayer session ended"
    exit 0
}

# Set trap for cleanup
trap cleanup EXIT INT TERM

# Run main function with all arguments
main "$@"
EOF

    chmod +x "$BUILD_DIR/run_wanjplayer.sh"
    print_success "Build directory runner script created and made executable"

    # Create build directory README
    create_build_readme
}

# Function to create build directory README
create_build_readme() {
    print_status "Creating build directory README..."

    cat > "$BUILD_DIR/README.md" << 'EOF'
# WanjPlayer Build Directory

This directory contains the compiled WanjPlayer executable and supporting files.

## Running WanjPlayer from Build Directory

**⚠️ IMPORTANT: Do not run `./WanjPlayer` directly!**

The bare executable will crash with a segmentation fault because it lacks proper environment configuration for video playback on modern Linux systems (especially Wayland).

### Recommended Method

Use the provided environment wrapper script:

```bash
./run_wanjplayer.sh
```

This script automatically:
- Detects your display system (Wayland/X11)
- Configures optimal video backends
- Sets up GStreamer for stable playback
- Disables problematic OpenGL acceleration
- Provides the same stability as the main launcher

### Alternative Method

Use the main launcher script from the project root:

```bash
cd ..
./wanjplayer
```

## Available Commands

### Basic Usage
```bash
./run_wanjplayer.sh              # Run WanjPlayer with environment setup
./run_wanjplayer.sh --info       # Show system information
./run_wanjplayer.sh --debug      # Run with debug output
./run_wanjplayer.sh --safe       # Run with maximum safety settings
./run_wanjplayer.sh --help       # Show all options
```

### System Information
```bash
./run_wanjplayer.sh --info
```

This shows:
- Detected display system (Wayland/X11/Hybrid)
- Video backend configuration
- Available display protocols
- Executable information

## Why This is Necessary

WanjPlayer uses GStreamer and wxWidgets for media playback, which requires specific environment configuration on modern Linux systems:

1. **Wayland Compatibility**: Forces X11 backend via XWayland for maximum compatibility
2. **OpenGL Management**: Disables problematic hardware acceleration that causes crashes
3. **GStreamer Setup**: Configures video sinks and plugin features for stable playback
4. **Display Backend**: Ensures proper display system integration

## Troubleshooting

### Segmentation Fault
- **Cause**: Running `./WanjPlayer` directly without environment setup
- **Solution**: Use `./run_wanjplayer.sh` instead

### Video Playback Issues
- **Cause**: Missing GStreamer plugins or incorrect video backend
- **Solution**: Run `./run_wanjplayer.sh --safe` for maximum compatibility

### Display Problems on Wayland
- **Cause**: Wayland/OpenGL compatibility issues
- **Solution**: The wrapper script automatically handles this by forcing X11 backend

## File Structure

```
build/
├── WanjPlayer              # Main executable (DO NOT RUN DIRECTLY)
├── run_wanjplayer.sh       # Environment wrapper (USE THIS)
├── README.md               # This file
├── assets/                 # Application assets
├── compile_commands.json   # Clang compilation database
└── [CMake build files]     # Build system files
```

## Development

If you're developing WanjPlayer, use the build system from the project root:

```bash
cd ..
./build.sh build           # Build the project
./build.sh run              # Build and run with environment setup
./build.sh info             # Show build system information
```

## Environment Variables Set by Wrapper

The wrapper script sets these variables for optimal compatibility:

**Wayland Systems:**
- `GDK_BACKEND=x11`
- `QT_QPA_PLATFORM=xcb`
- `GST_GL_DISABLED=1`
- `LIBGL_ALWAYS_SOFTWARE=1`
- `GST_VIDEO_SINK=ximagesink`

**X11 Systems:**
- `GST_GL_DISABLED=1`
- `GST_VIDEO_SINK=ximagesink`
- `GST_DEBUG=1`

## Support

For issues or questions:
- Check `./run_wanjplayer.sh --help` for all options
- Use `./run_wanjplayer.sh --info` to diagnose your system
- Run `../build.sh info` for comprehensive build system information

---

**Remember: Always use `./run_wanjplayer.sh` instead of `./WanjPlayer` directly!**
EOF

    print_success "Build directory README created"
}

# Function to run the application with proper environment
run_application() {
    print_status "Running WanjPlayer with optimized environment..."

    if [ ! -f "$BUILD_DIR/WanjPlayer" ]; then
        print_error "WanjPlayer executable not found. Build the project first."
        return 1
    fi

    # Configure runtime environment based on detected system
    case "$DETECTED_SYSTEM" in
        "wayland"|"hybrid")
            print_info "Configuring Wayland-compatible runtime environment..."

            # Force X11 backend through XWayland for maximum compatibility
            export GDK_BACKEND=x11
            export QT_QPA_PLATFORM=xcb

            # Disable all OpenGL acceleration to prevent segfaults
            export GST_GL_DISABLED=1
            export LIBGL_ALWAYS_SOFTWARE=1
            export MESA_GL_VERSION_OVERRIDE=2.1

            # Configure GStreamer for stable video playback
            export GST_VIDEO_SINK=ximagesink
            export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload"
            export GST_DEBUG=1
            export GST_REGISTRY_FORK=no
            ;;

        "x11"|"x11_only")
            print_info "Configuring X11 native runtime environment..."

            # Minimal safe settings for X11
            export GST_GL_DISABLED=1
            export GST_VIDEO_SINK=ximagesink
            export GST_DEBUG=1
            ;;

        *)
            print_warning "Configuring conservative runtime environment..."

            # Most conservative settings for unknown systems
            export GST_GL_DISABLED=1
            export LIBGL_ALWAYS_SOFTWARE=1
            export GST_VIDEO_SINK=ximagesink
            export GST_DEBUG=2
            ;;
    esac

    cd "$BUILD_DIR"
    print_info "Starting WanjPlayer..."
    print_info "Press Ctrl+C to exit"
    echo

    exec ./WanjPlayer
}

# Function to format code
format_code() {
    print_status "Formatting code with clang-format..."

    if ! command_exists clang-format; then
        print_error "clang-format not found. Install it first."
        return 1
    fi

    find "$PROJECT_ROOT/src" "$PROJECT_ROOT/include" -name "*.cpp" -o -name "*.hpp" 2>/dev/null | \
        xargs clang-format -i --style=file
    print_success "Code formatted successfully"
}

# Function to check code with clang-tidy
check_code() {
    print_status "Running clang-tidy checks..."

    if ! command_exists clang-tidy; then
        print_error "clang-tidy not found. Install it first."
        return 1
    fi

    if [ ! -f "$PROJECT_ROOT/compile_commands.json" ]; then
        print_error "compile_commands.json not found. Build the project first."
        return 1
    fi

    find "$PROJECT_ROOT/src" -name "*.cpp" 2>/dev/null | \
        xargs clang-tidy --config-file="$PROJECT_ROOT/.clangd" 2>/dev/null || true
    print_success "Code analysis completed"
}

# Function to install dependencies (for Ubuntu/Debian)
install_deps() {
    print_status "Installing build dependencies for Ubuntu/Debian..."

    if ! command_exists apt; then
        print_error "This system doesn't appear to use apt. Please install dependencies manually."
        print_info "Required packages: build-essential cmake clang clang-format clang-tidy"
        print_info "wxWidgets: libwxgtk3.2-dev libwxbase3.2-dev"
        print_info "GStreamer: libgstreamer1.0-dev gst-plugins-base gst-plugins-good"
        return 1
    fi

    sudo apt update
    sudo apt install -y \
        build-essential \
        cmake \
        clang \
        clang-format \
        clang-tidy \
        libwxgtk3.2-dev \
        libwxbase3.2-dev \
        libgstreamer1.0-dev \
        gstreamer1.0-plugins-base \
        gstreamer1.0-plugins-good \
        gstreamer1.0-plugins-bad \
        gstreamer1.0-tools \
        pkg-config

    print_success "Dependencies installed successfully"
}

# Function to show system information
show_system_info() {
    detect_environment
    check_dependencies
    check_gstreamer

    echo
    print_info "System Information:"
    echo "  • Display System: $DETECTED_SYSTEM"
    echo "  • Build Mode: $VIDEO_MODE"
    echo "  • Wayland: $([ -n "$WAYLAND_DISPLAY" ] && echo "Available" || echo "Not available")"
    echo "  • X11: $([ -n "$X11_DISPLAY" ] && echo "Available" || echo "Not available")"
    echo "  • CMake: $(command_exists cmake && cmake --version | head -n1 | cut -d' ' -f3 || echo "Not found")"
    echo "  • GCC: $(command_exists g++ && g++ --version | head -n1 || echo "Not found")"
    echo "  • wxWidgets: $(command_exists wx-config && wx-config --version || echo "Not found")"
    echo "  • GStreamer: $(command_exists gst-inspect-1.0 && echo "Available" || echo "Not found")"
    echo "  • CPU Cores: $(nproc)"
}

# Function to show help
show_help() {
    echo "$SCRIPT_NAME v$VERSION"
    echo "Enhanced build system with environment detection and optimization"
    echo
    echo "Usage: $0 [COMMAND] [OPTIONS]"
    echo
    echo "Commands:"
    echo "  build          Build the project (default)"
    echo "  clean          Clean build directory"
    echo "  rebuild        Clean and build the project"
    echo "  configure      Configure CMake without building"
    echo "  run            Build (if needed) and run the application"
    echo "  format         Format code with clang-format"
    echo "  check          Run clang-tidy analysis"
    echo "  deps           Install build dependencies (Ubuntu/Debian)"
    echo "  info           Show system and environment information"
    echo "  help           Show this help message"
    echo
    echo "Options:"
    echo "  --verbose      Enable verbose output"
    echo "  --parallel N   Set parallel build jobs (default: auto-detect)"
    echo
    echo "Examples:"
    echo "  $0                    # Build the project"
    echo "  $0 rebuild            # Clean and rebuild"
    echo "  $0 run                # Build and run with environment setup"
    echo "  $0 --parallel 4 build # Build with 4 parallel jobs"
    echo "  $0 info               # Show system information"
    echo
    echo "Environment Detection:"
    echo "  This script automatically detects your display system (Wayland/X11)"
    echo "  and configures the build environment for optimal compatibility."
}

# Function to show version
show_version() {
    echo "$SCRIPT_NAME $VERSION"
    echo "Copyright (c) 2024 $AUTHOR"
    echo "Enhanced build system for cross-platform media player development"
}

# Main execution logic
main() {
    local command="build"
    local verbose=false
    local parallel_jobs=""

    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --verbose)
                verbose=true
                set -x
                shift
                ;;
            --parallel)
                parallel_jobs="$2"
                export CMAKE_BUILD_PARALLEL_LEVEL="$parallel_jobs"
                shift 2
                ;;
            -h|--help)
                show_help
                exit 0
                ;;
            -v|--version)
                show_version
                exit 0
                ;;
            info)
                command="info"
                shift
                ;;
            clean|configure|build|rebuild|run|format|check|deps)
                command="$1"
                shift
                ;;
            *)
                print_error "Unknown option: $1"
                print_info "Use --help for usage information"
                exit 1
                ;;
        esac
    done

    # Show header for most commands
    if [ "$command" != "info" ] && [ "$command" != "help" ]; then
        print_header
        echo
    fi

    # Detect environment for build-related commands
    if [[ "$command" =~ ^(build|rebuild|run|configure)$ ]]; then
        detect_environment
        configure_build_environment
        echo
    fi

    # Execute the requested command
    case "$command" in
        "info")
            print_header
            echo
            show_system_info
            ;;
        "clean")
            clean_build
            ;;
        "configure")
            check_dependencies || print_warning "Some dependencies are missing"
            configure_project
            ;;
        "build")
            check_dependencies || print_warning "Some dependencies are missing"
            if [ ! -d "$BUILD_DIR" ]; then
                configure_project
            fi
            build_project
            update_compile_commands
            ;;
        "rebuild")
            check_dependencies || print_warning "Some dependencies are missing"
            clean_build
            configure_project
            build_project
            update_compile_commands
            ;;
        "run")
            check_dependencies || print_warning "Some dependencies are missing"
            if [ ! -f "$BUILD_DIR/WanjPlayer" ]; then
                print_warning "Executable not found. Building first..."
                if [ ! -d "$BUILD_DIR" ]; then
                    configure_project
                fi
                build_project
                update_compile_commands
            fi
            run_application
            ;;
        "format")
            format_code
            ;;
        "check")
            check_code
            ;;
        "deps")
            install_deps
            ;;
        *)
            print_error "Unknown command: $command"
            show_help
            exit 1
            ;;
    esac
}

# Cleanup function for graceful exit
cleanup() {
    if [ -n "${CMAKE_BUILD_PARALLEL_LEVEL:-}" ]; then
        unset CMAKE_BUILD_PARALLEL_LEVEL
    fi
}

# Set trap for cleanup
trap cleanup EXIT INT TERM

# Check if we're in the right directory
if [ ! -f "$PROJECT_ROOT/CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Are you in the project root?"
    exit 1
fi

# Run main function with all arguments
main "$@"
