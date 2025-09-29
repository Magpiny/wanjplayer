#!/bin/bash
# GStreamer Video Test Script for Wayland
# This script tests different GStreamer pipelines to identify what works

echo "GStreamer Video Pipeline Test for Wayland"
echo "=========================================="

# Test 1: Basic waylandsink test
echo ""
echo "Test 1: Testing waylandsink with test video..."
echo "Command: gst-launch-1.0 videotestsrc num-buffers=300 ! waylandsink"
timeout 5s gst-launch-1.0 videotestsrc num-buffers=300 ! waylandsink 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✓ waylandsink works"
else
    echo "✗ waylandsink failed"
fi

# Test 2: Software-only pipeline
echo ""
echo "Test 2: Testing software-only video pipeline..."
echo "Command: gst-launch-1.0 videotestsrc num-buffers=300 ! videoconvert ! waylandsink"
timeout 5s gst-launch-1.0 videotestsrc num-buffers=300 ! videoconvert ! waylandsink 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✓ software pipeline with waylandsink works"
else
    echo "✗ software pipeline with waylandsink failed"
fi

# Test 3: ximagesink fallback
echo ""
echo "Test 3: Testing ximagesink fallback..."
echo "Command: gst-launch-1.0 videotestsrc num-buffers=300 ! ximagesink"
timeout 5s gst-launch-1.0 videotestsrc num-buffers=300 ! ximagesink 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✓ ximagesink works"
else
    echo "✗ ximagesink failed"
fi

# Test 4: Check for GL plugins
echo ""
echo "Test 4: Checking for problematic GL plugins..."
GL_PLUGINS=$(gst-inspect-1.0 | grep -i "gl" | wc -l)
echo "Found $GL_PLUGINS GL-related plugins"

# Test 5: Test actual video file (if provided)
if [ "$1" != "" ] && [ -f "$1" ]; then
    echo ""
    echo "Test 5: Testing actual video file: $1"

    echo "5a: Testing with waylandsink..."
    timeout 10s gst-launch-1.0 filesrc location="$1" ! decodebin ! videoconvert ! waylandsink 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "✓ Video file works with waylandsink"
    else
        echo "✗ Video file failed with waylandsink"
    fi

    echo "5b: Testing with ximagesink..."
    timeout 10s gst-launch-1.0 filesrc location="$1" ! decodebin ! videoconvert ! ximagesink 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "✓ Video file works with ximagesink"
    else
        echo "✗ Video file failed with ximagesink"
    fi
fi

# Test 6: Check environment variables
echo ""
echo "Test 6: Current environment variables..."
echo "WAYLAND_DISPLAY: $WAYLAND_DISPLAY"
echo "XDG_SESSION_TYPE: $XDG_SESSION_TYPE"
echo "GST_DEBUG: $GST_DEBUG"
echo "LIBGL_ALWAYS_SOFTWARE: $LIBGL_ALWAYS_SOFTWARE"

# Test 7: Recommended pipeline for wxWidgets
echo ""
echo "Test 7: Recommended working pipeline found:"
echo "For waylandsink: filesrc ! decodebin ! videoconvert ! waylandsink"
echo "For ximagesink: filesrc ! decodebin ! videoconvert ! ximagesink"

echo ""
echo "Test completed. If waylandsink works, your system should support video playback."
echo "Usage: ./test_video.sh [path_to_video_file]"
