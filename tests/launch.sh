#!/bin/bash
# WanjPlayer Universal Launcher
# Automatically configures the best video backend for your system

echo "WanjPlayer - Universal Media Player"
echo "=================================="

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check if WanjPlayer is built
if [ ! -f "./build/WanjPlayer" ]; then
    echo "Building WanjPlayer..."
    if ! cmake --build build >/dev/null 2>&1; then
        echo "ERROR: Failed to build WanjPlayer!"
        echo "Please run: cmake --build build"
        exit 1
    fi
    echo "✓ Build completed"
fi

# Detect system environment
echo "Detecting system environment..."
SESSION_TYPE="${XDG_SESSION_TYPE:-unknown}"
WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-}"
X11_DISPLAY="${DISPLAY:-}"

echo "Session Type: $SESSION_TYPE"
if [ -n "$WAYLAND_DISPLAY" ]; then
    echo "Wayland: Available"
fi
if [ -n "$X11_DISPLAY" ]; then
    echo "X11: Available"
fi

# Auto-configure based on environment
if [ "$SESSION_TYPE" = "wayland" ] || [ -n "$WAYLAND_DISPLAY" ]; then
    echo ""
    echo "Wayland detected - Configuring for video compatibility..."

    # Apply the working Wayland configuration
    export GDK_BACKEND=x11
    export QT_QPA_PLATFORM=xcb
    export GST_GL_DISABLED=1
    export LIBGL_ALWAYS_SOFTWARE=1
    export GST_VIDEO_SINK=ximagesink
    export GST_PLUGIN_FEATURE_DISABLE="glimagesink,glsinkbin,gtkglsink,glcolorconvert,glupload,gldownload"
    export GST_DEBUG=1
    export MESA_GL_VERSION_OVERRIDE=2.1

    echo "✓ Wayland video compatibility mode enabled"
    echo "✓ Using X11 backend through XWayland"
    echo "✓ OpenGL acceleration disabled for stability"

elif [ "$SESSION_TYPE" = "x11" ] || [ -n "$X11_DISPLAY" ]; then
    echo ""
    echo "X11 detected - Using optimized configuration..."

    export GST_GL_DISABLED=1
    export GST_VIDEO_SINK=ximagesink
    export GST_DEBUG=1

    echo "✓ X11 video backend configured"

else
    echo ""
    echo "Unknown environment - Using safe defaults..."

    export GST_GL_DISABLED=1
    export LIBGL_ALWAYS_SOFTWARE=1
    export GST_DEBUG=2

    echo "✓ Safe fallback configuration applied"
fi

echo ""
echo "Starting WanjPlayer..."
echo "Press Ctrl+C to exit"
echo ""

# Navigate to the correct directory
cd "$(dirname "$0")"

# Launch WanjPlayer
exec ./build/WanjPlayer
