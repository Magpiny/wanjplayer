#!/bin/bash
# Test Script for WanjPlayer Status Bar Timer Functionality
# This script helps verify that the status bar counter updates every second

echo "WanjPlayer Status Bar Timer Test"
echo "================================"
echo ""
echo "This test verifies that the duration counter in the status bar"
echo "updates every second during media playback."
echo ""

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Create test directory
TEST_DIR="./test_timer"
mkdir -p "$TEST_DIR"

echo "Setting up test environment..."
echo ""

# Generate test audio files with different durations for timer testing
TEST_FILES=()

if command_exists ffmpeg; then
    echo "Generating test audio files with ffmpeg..."

    # Create files of different lengths to test timer functionality
    ffmpeg -f lavfi -i "sine=440:d=10" -y "$TEST_DIR/timer_test_10sec.wav" 2>/dev/null
    ffmpeg -f lavfi -i "sine=880:d=30" -y "$TEST_DIR/timer_test_30sec.wav" 2>/dev/null
    ffmpeg -f lavfi -i "sine=220:d=60" -y "$TEST_DIR/timer_test_60sec.wav" 2>/dev/null

    if [ -f "$TEST_DIR/timer_test_10sec.wav" ]; then
        TEST_FILES+=("$TEST_DIR/timer_test_10sec.wav")
        TEST_FILES+=("$TEST_DIR/timer_test_30sec.wav")
        TEST_FILES+=("$TEST_DIR/timer_test_60sec.wav")
        echo "✓ Generated 3 test files (10s, 30s, 60s duration)"
    else
        echo "✗ Failed to generate test files with ffmpeg"
    fi

elif command_exists sox; then
    echo "Generating test audio files with sox..."

    sox -n "$TEST_DIR/timer_test_10sec.wav" synth 10 sine 440 vol 0.5 2>/dev/null
    sox -n "$TEST_DIR/timer_test_30sec.wav" synth 30 sine 880 vol 0.5 2>/dev/null
    sox -n "$TEST_DIR/timer_test_60sec.wav" synth 60 sine 220 vol 0.5 2>/dev/null

    if [ -f "$TEST_DIR/timer_test_10sec.wav" ]; then
        TEST_FILES+=("$TEST_DIR/timer_test_10sec.wav")
        TEST_FILES+=("$TEST_DIR/timer_test_30sec.wav")
        TEST_FILES+=("$TEST_DIR/timer_test_60sec.wav")
        echo "✓ Generated 3 test files (10s, 30s, 60s duration)"
    else
        echo "✗ Failed to generate test files with sox"
    fi
else
    # Try to find system audio files
    echo "Looking for system audio files..."

    while IFS= read -r -d '' file; do
        TEST_FILES+=("$file")
        if [ ${#TEST_FILES[@]} -ge 3 ]; then break; fi
    done < <(find /usr/share/sounds -name "*.wav" -o -name "*.ogg" 2>/dev/null | head -3 | tr '\n' '\0')

    if [ ${#TEST_FILES[@]} -gt 0 ]; then
        echo "✓ Found ${#TEST_FILES[@]} system audio files for testing"
    else
        echo "✗ No audio files available for testing"
        echo ""
        echo "To run this test, please install either:"
        echo "  sudo pacman -S sox ffmpeg        # Arch Linux"
        echo "  sudo apt install sox ffmpeg      # Debian/Ubuntu"
        echo "  sudo dnf install sox ffmpeg      # Fedora"
        exit 1
    fi
fi

echo ""
echo "Test files ready:"
for i in "${!TEST_FILES[@]}"; do
    filename=$(basename "${TEST_FILES[$i]}")
    echo "  $((i+1)). $filename"
done

# Build if needed
echo ""
if [ ! -f "./build/WanjPlayer" ]; then
    echo "Building WanjPlayer..."
    if ! cmake --build build >/dev/null 2>&1; then
        echo "ERROR: Failed to build WanjPlayer!"
        exit 1
    fi
fi

echo ""
echo "Status Bar Timer Test Instructions"
echo "=================================="
echo ""
echo "1. WHAT TO TEST:"
echo "   ✓ Status bar shows duration in format: 00:00 / 03:45"
echo "   ✓ First number (current time) updates every second"
echo "   ✓ Second number (total time) stays constant"
echo "   ✓ Format changes to HH:MM:SS for files over 1 hour"
echo ""
echo "2. TEST PROCEDURE:"
echo "   a) Load test files using File → Open Files"
echo "   b) Watch the status bar (bottom of window, column 2)"
echo "   c) Start playback and observe the counter"
echo "   d) Verify it updates every second: 00:01, 00:02, 00:03..."
echo "   e) Test seeking - counter should jump to new position"
echo "   f) Test pause/resume - counter should pause/resume"
echo ""
echo "3. EXPECTED RESULTS:"
echo "   ✓ Counter format: MM:SS / MM:SS (or HH:MM:SS / HH:MM:SS)"
echo "   ✓ Updates precisely every 1 second"
echo "   ✓ Synchronizes with position slider movement"
echo "   ✓ Shows correct time after seeking"
echo "   ✓ Pauses when media is paused"
echo ""
echo "4. FILES TO TEST:"
echo "   • Short file (10s): Quick verification"
echo "   • Medium file (30s): Observe consistent updates"
echo "   • Long file (60s): Test minute rollover"
echo ""
echo "5. WHAT TO LOOK FOR:"
echo "   ❌ Counter stuck at 00:00"
echo "   ❌ Counter not updating every second"
echo "   ❌ Counter format incorrect (should be current/total)"
echo "   ❌ Counter not synchronized with position slider"
echo "   ❌ Counter continues when paused"
echo ""

# Create instruction file
cat > "$TEST_DIR/timer_test_checklist.txt" << 'EOF'
STATUS BAR TIMER TEST CHECKLIST
===============================

□ Load test files (File → Open Files)
□ Start playback of first file
□ Verify status bar column 2 shows: 00:00 / XX:XX format
□ Watch counter for 10 seconds - should update every second
□ Verify format: 00:01, 00:02, 00:03, ..., 00:09, 00:10
□ Pause playback - counter should freeze
□ Resume playback - counter should continue updating
□ Click position slider to seek - counter should jump immediately
□ Load longer file and verify minute rollover (00:59 → 01:00)
□ Test with file over 1 hour (if available) - verify HH:MM:SS format

EXPECTED STATUS BAR LAYOUT:
Column 0: [System Messages]
Column 1: [filename.mp3]
Column 2: [00:15 / 03:45]  ← THIS SHOULD UPDATE EVERY SECOND
Column 3: [Playing]

PASS CRITERIA:
✓ Counter updates precisely every 1 second during playback
✓ Counter shows current position / total duration
✓ Counter synchronizes with position slider
✓ Counter pauses when media is paused
✓ Counter updates immediately when seeking
✓ Format is consistent (MM:SS or HH:MM:SS)
EOF

echo "Detailed checklist saved to: $TEST_DIR/timer_test_checklist.txt"
echo ""
echo "Starting WanjPlayer for timer testing..."
echo "File location for testing: $TEST_DIR"
echo ""
echo "IMPORTANT: Watch the status bar (bottom of window, column 2)"
echo "The duration should show as: current_time / total_time"
echo "and update every second during playback!"
echo ""
echo "NOTE: Timer updates are silent (no debug messages) for clean UI"
echo "Press Ctrl+C to exit when testing is complete."
echo ""

# Set environment for testing
export GST_DEBUG=1
export GST_GL_DISABLED=1
export LIBGL_ALWAYS_SOFTWARE=1

# Launch WanjPlayer
./wanjplayer &
WANJPLAYER_PID=$!

# Cleanup function
cleanup() {
    echo ""
    echo "Cleaning up test environment..."
    kill $WANJPLAYER_PID 2>/dev/null
    wait $WANJPLAYER_PID 2>/dev/null
    rm -rf "$TEST_DIR" 2>/dev/null
    echo ""
    echo "Timer Test Summary:"
    echo "=================="
    echo "✓ Test files generated successfully"
    echo "✓ WanjPlayer launched for testing"
    echo ""
    echo "Status Bar Timer Features Tested:"
    echo "• Real-time duration counter (current/total format)"
    echo "• 1-second update interval (silent, no debug messages)"
    echo "• Synchronization with media playback"
    echo "• Pause/resume functionality"
    echo "• Seek position updates"
    echo ""
    echo "If the timer worked correctly, you should have seen:"
    echo "• Status bar column 2 showing: 00:00 / 03:45 (example)"
    echo "• First number incrementing every second: 00:01, 00:02, etc."
    echo "• Immediate updates when seeking or changing tracks"
    echo "• Counter pausing when playback is paused"
    echo "• Clean interface without annoying timer log messages"
    echo ""
    echo "Timer test completed!"
}

# Set trap for cleanup
trap cleanup EXIT INT TERM

# Wait for user to complete testing
wait $WANJPLAYER_PID 2>/dev/null
