#!/bin/bash
# Clean GStreamer registry and run WanjPlayer with minimal GL support
# This script helps resolve GStreamer GL conflicts on Wayland

echo "Cleaning GStreamer registry to remove problematic GL plugins..."

# Remove existing GStreamer registry
rm -rf ~/.cache/gstreamer-1.0/registry.*.bin
rm -rf ~/.cache/gstreamer-1.0/registry.*.bin.tmp*

# Set environment variables to disable GL completely
export GST_GL_DISABLED=1
export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload,glcolorbalance,glvideoflip,glimagesinkelement"
export LIBGL_ALWAYS_SOFTWARE=1
export GST_DEBUG=2

# Force software video sink
export GST_VIDEOSINK=waylandsink

# Alternative sinks to try if waylandsink fails
# export GST_VIDEOSINK=ximagesink
# export GST_VIDEOSINK=xvimagesink

# Regenerate registry without GL plugins
echo "Regenerating GStreamer registry..."
gst-inspect-1.0 --print-plugin-auto-install-info > /dev/null 2>&1

echo "Available video sinks:"
gst-inspect-1.0 | grep -E "(video.*sink|waylandsink|ximagesink)"

echo ""
echo "Starting WanjPlayer with clean registry and software rendering..."

# Navigate to the correct directory
cd "$(dirname "$0")"

# Run the application
if [ -f "./build/WanjPlayer" ]; then
    ./build/WanjPlayer
elif [ -f "./WanjPlayer" ]; then
    ./WanjPlayer
else
    echo "Error: WanjPlayer executable not found!"
    echo "Please build the project first using: cmake --build build"
    exit 1
fi
