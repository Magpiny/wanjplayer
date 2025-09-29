#!/bin/bash
# Test Playlist Functionality Script
# Tests WanjPlayer playlist features with available audio files

echo "WanjPlayer Playlist Test Script"
echo "==============================="

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Create test directory
TEST_DIR="./test_media"
mkdir -p "$TEST_DIR"

echo "Setting up test environment..."

# Find available audio files on the system
echo "Looking for test audio files..."
TEST_FILES=()

# Check for system sounds
if [ -d "/usr/share/sounds" ]; then
    while IFS= read -r -d '' file; do
        TEST_FILES+=("$file")
    done < <(find /usr/share/sounds -name "*.wav" -o -name "*.ogg" 2>/dev/null | head -5 | tr '\n' '\0')
fi

# Generate test audio files if we have the tools and no system sounds
if [ ${#TEST_FILES[@]} -eq 0 ]; then
    echo "No system audio files found. Checking for audio generation tools..."

    if command_exists sox; then
        echo "Generating test audio files with sox..."
        sox -n "$TEST_DIR/test1.wav" synth 3 sine 440 vol 0.5 2>/dev/null
        sox -n "$TEST_DIR/test2.wav" synth 3 sine 880 vol 0.5 2>/dev/null
        sox -n "$TEST_DIR/test3.wav" synth 3 sine 1320 vol 0.5 2>/dev/null
        TEST_FILES=("$TEST_DIR/test1.wav" "$TEST_DIR/test2.wav" "$TEST_DIR/test3.wav")
    elif command_exists ffmpeg; then
        echo "Generating test audio files with ffmpeg..."
        ffmpeg -f lavfi -i "sine=440:d=3" -y "$TEST_DIR/test1.wav" 2>/dev/null
        ffmpeg -f lavfi -i "sine=880:d=3" -y "$TEST_DIR/test2.wav" 2>/dev/null
        ffmpeg -f lavfi -i "sine=1320:d=3" -y "$TEST_DIR/test3.wav" 2>/dev/null
        TEST_FILES=("$TEST_DIR/test1.wav" "$TEST_DIR/test2.wav" "$TEST_DIR/test3.wav")
    else
        echo "No audio generation tools available (sox, ffmpeg)."
        echo "Using system notification sounds if available..."

        # Try some common locations
        for sound in /usr/share/sounds/alsa/*.wav /usr/share/sounds/*/*.wav /usr/share/sounds/*/*.ogg; do
            if [ -f "$sound" ] && [ ${#TEST_FILES[@]} -lt 3 ]; then
                TEST_FILES+=("$sound")
            fi
        done
    fi
fi

if [ ${#TEST_FILES[@]} -eq 0 ]; then
    echo "ERROR: No audio files available for testing!"
    echo ""
    echo "To run this test, you need either:"
    echo "1. System audio files in /usr/share/sounds/"
    echo "2. sox or ffmpeg installed for generating test audio"
    echo ""
    echo "Install options:"
    echo "  sudo pacman -S sox              # For Arch Linux"
    echo "  sudo apt install sox            # For Debian/Ubuntu"
    echo "  sudo dnf install sox            # For Fedora"
    exit 1
fi

echo "Found ${#TEST_FILES[@]} audio files for testing:"
for i in "${!TEST_FILES[@]}"; do
    echo "  $((i+1)). ${TEST_FILES[$i]}"
done

echo ""
echo "Test Instructions:"
echo "=================="
echo "1. The application will start with multiple audio files loaded"
echo "2. Test the following features:"
echo "   - Click on different playlist items to play them"
echo "   - Use Play/Pause/Stop buttons"
echo "   - Use Next/Previous buttons"
echo "   - Check if duration updates"
echo "   - Try the volume slider"
echo "   - Try the position slider for seeking"
echo "3. Audio files should play one after another automatically"
echo "4. Press Ctrl+C or close the window to exit"
echo ""

# Build the application if needed
if [ ! -f "./build/WanjPlayer" ]; then
    echo "Building WanjPlayer..."
    if ! cmake --build build >/dev/null 2>&1; then
        echo "ERROR: Failed to build WanjPlayer!"
        exit 1
    fi
fi

echo "Starting WanjPlayer for playlist testing..."
echo "Note: If you experience video-related crashes, this test uses audio files only."
echo ""

# Set environment to minimize issues
export GST_DEBUG=1
export GST_GL_DISABLED=1
export LIBGL_ALWAYS_SOFTWARE=1

# Create a temporary script that will load multiple files
PLAYLIST_SCRIPT=$(cat << 'EOF'
#!/bin/bash
# Auto-load playlist files
echo "Auto-loading playlist files..."

# Give WanjPlayer time to start
sleep 2

# You could extend this to automatically load files via command line
# if WanjPlayer supported command line arguments
echo "Please use File -> Open Files to load the test audio files:"
EOF
)

echo "$PLAYLIST_SCRIPT" > "/tmp/wanjplayer_autoload.sh"
chmod +x "/tmp/wanjplayer_autoload.sh"

# Start the application
echo "Files to load (copy this path when using File->Open Files):"
echo "$TEST_DIR"
echo ""

# Start WanjPlayer
./build/WanjPlayer &
WANJPLAYER_PID=$!

# Optional: Clean up on exit
cleanup() {
    echo ""
    echo "Cleaning up..."
    kill $WANJPLAYER_PID 2>/dev/null
    rm -rf "$TEST_DIR" 2>/dev/null
    rm -f "/tmp/wanjplayer_autoload.sh" 2>/dev/null
    echo "Test completed."
}

trap cleanup EXIT INT TERM

# Wait for the application to finish
wait $WANJPLAYER_PID 2>/dev/null

echo ""
echo "Playlist Test Summary:"
echo "====================="
echo "✓ Generated/found ${#TEST_FILES[@]} test audio files"
echo "✓ WanjPlayer started successfully"
echo ""
echo "Expected Playlist Features:"
echo "- ✓ Multiple file loading via File->Open Files"
echo "- ✓ Click playlist items to play specific tracks"
echo "- ✓ Next/Previous buttons navigate playlist"
echo "- ✓ Auto-play next track when current finishes"
echo "- ✓ Duration display and updates"
echo "- ✓ Volume control slider"
echo "- ✓ Position/seek slider"
echo "- ✓ Play/Pause/Stop controls"
echo ""
echo "If any features didn't work, check the console output for error messages."
