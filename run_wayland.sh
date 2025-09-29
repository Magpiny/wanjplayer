#!/bin/bash
# Wayland-compatible startup script for WanjPlayer
# This script sets the appropriate GStreamer environment variables for Wayland

# Force GStreamer to use software rendering and disable GL acceleration
export GST_GL_PLATFORM=""
export GST_GL_API=""
export GST_GL_WINDOW=""

# Disable hardware acceleration completely
export LIBGL_ALWAYS_SOFTWARE=1
export GST_DEBUG=3

# Force software video sink for Wayland
export GST_VIDEO_SINK=waylandsink

# Disable GL elements in GStreamer
export GST_ELEMENT_DISABLE="glimagesink,glsinkbin,gtkglsink"

# Alternative: Try with xvimagesink if waylandsink doesn't work
# export GST_VIDEO_SINK=xvimagesink

# Set Wayland-specific variables
export WAYLAND_DISPLAY=${WAYLAND_DISPLAY:-wayland-0}

# Run WanjPlayer
cd "$(dirname "$0")"
if [ -f "./build/WanjPlayer" ]; then
    echo "Starting WanjPlayer with Wayland-compatible GStreamer settings..."
    ./build/WanjPlayer
elif [ -f "./WanjPlayer" ]; then
    echo "Starting WanjPlayer with Wayland-compatible GStreamer settings..."
    ./WanjPlayer
else
    echo "Error: WanjPlayer executable not found!"
    echo "Please build the project first using: cmake --build build"
    exit 1
fi
