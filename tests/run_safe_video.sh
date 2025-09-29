#!/bin/bash
# WanjPlayer Safe Video Startup Script
# Comprehensive solution for GStreamer GL conflicts on Wayland
# This script tries multiple approaches to get video playback working

echo "WanjPlayer - Safe Video Startup Script"
echo "======================================"

# Function to test if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to test GStreamer video sink
test_gst_sink() {
    local sink=$1
    echo "Testing GStreamer sink: $sink"
    timeout 3s gst-launch-1.0 videotestsrc num-buffers=30 ! $sink >/dev/null 2>&1
    return $?
}

# Function to run WanjPlayer with specific environment
run_wanjplayer() {
    echo "Starting WanjPlayer..."
    cd "$(dirname "$0")"
    if [ -f "./build/WanjPlayer" ]; then
        ./build/WanjPlayer
    elif [ -f "./WanjPlayer" ]; then
        ./WanjPlayer
    else
        echo "Error: WanjPlayer executable not found!"
        echo "Please build the project first: cmake --build build"
        exit 1
    fi
}

# Check system information
echo ""
echo "System Information:"
echo "XDG_SESSION_TYPE: $XDG_SESSION_TYPE"
echo "WAYLAND_DISPLAY: $WAYLAND_DISPLAY"
echo "DISPLAY: $DISPLAY"

# Strategy 1: Force X11 backend (most compatible)
echo ""
echo "Strategy 1: Using X11 backend through XWayland..."
export GDK_BACKEND=x11
export QT_QPA_PLATFORM=xcb
export GST_GL_DISABLED=1
export LIBGL_ALWAYS_SOFTWARE=1
export GST_DEBUG=1
export GST_VIDEO_SINK=ximagesink
export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload"

# Test if ximagesink works
if test_gst_sink "ximagesink"; then
    echo "✓ ximagesink test passed, launching WanjPlayer with X11 backend..."
    run_wanjplayer
    exit $?
else
    echo "✗ ximagesink test failed, trying next strategy..."
fi

# Strategy 2: Pure Wayland with software rendering
echo ""
echo "Strategy 2: Pure Wayland with software rendering..."
unset GDK_BACKEND
unset QT_QPA_PLATFORM
export GST_GL_DISABLED=1
export LIBGL_ALWAYS_SOFTWARE=1
export GST_DEBUG=1
export GST_VIDEO_SINK=waylandsink
export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload,gleffects"

# Test if waylandsink works
if test_gst_sink "waylandsink"; then
    echo "✓ waylandsink test passed, launching WanjPlayer with Wayland..."
    run_wanjplayer
    exit $?
else
    echo "✗ waylandsink test failed, trying next strategy..."
fi

# Strategy 3: Disable all hardware acceleration
echo ""
echo "Strategy 3: Complete software fallback..."
export GDK_BACKEND=x11
export LIBGL_ALWAYS_SOFTWARE=1
export MESA_GL_VERSION_OVERRIDE=2.1
export GST_GL_DISABLED=1
export GST_DEBUG=1
export GST_REGISTRY_FORK=no
unset GST_VIDEO_SINK
export GST_PLUGIN_FEATURE_DISABLE="gl,opengl,vaapi,vdpau,nvdec,nvenc"

# Clean GStreamer registry
echo "Cleaning GStreamer registry..."
rm -rf ~/.cache/gstreamer-1.0/registry.*.bin* 2>/dev/null

echo "✓ Software fallback configured, launching WanjPlayer..."
run_wanjplayer
exit $?

# If we get here, all strategies failed
echo ""
echo "All strategies failed. Troubleshooting information:"
echo "=================================================="

echo ""
echo "Available GStreamer video sinks:"
gst-inspect-1.0 | grep -E "sink.*Video" | head -5

echo ""
echo "GStreamer GL plugins (these might be causing issues):"
gst-inspect-1.0 | grep -i gl | head -5

echo ""
echo "Recommendations:"
echo "1. Install missing GStreamer plugins:"
echo "   sudo pacman -S gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad"
echo ""
echo "2. For audio-only playback, the application should work fine."
echo ""
echo "3. Try running with debug output:"
echo "   GST_DEBUG=3 ./build/WanjPlayer"
echo ""
echo "4. Consider using VLC as an alternative:"
echo "   sudo pacman -S vlc"

exit 1
