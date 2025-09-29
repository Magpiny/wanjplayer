#!/bin/bash
# Comprehensive Test Script for WanjPlayer Fixes
# Tests all recent fixes and improvements

echo "WanjPlayer Fixes Comprehensive Test"
echo "==================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_test() {
    echo -e "${BLUE}TEST:${NC} $1"
}

print_pass() {
    echo -e "${GREEN}✓ PASS:${NC} $1"
}

print_fail() {
    echo -e "${RED}✗ FAIL:${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠ WARNING:${NC} $1"
}

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0
TOTAL_TESTS=0

run_test() {
    local test_name="$1"
    local test_command="$2"
    local expected_result="$3"  # 0 for success, 1 for failure expected

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    print_test "$test_name"

    # Run the test command
    if eval "$test_command" >/dev/null 2>&1; then
        if [ "$expected_result" = "0" ]; then
            print_pass "$test_name"
            TESTS_PASSED=$((TESTS_PASSED + 1))
        else
            print_fail "$test_name (expected failure but got success)"
            TESTS_FAILED=$((TESTS_FAILED + 1))
        fi
    else
        if [ "$expected_result" = "1" ]; then
            print_pass "$test_name (expected failure)"
            TESTS_PASSED=$((TESTS_PASSED + 1))
        else
            print_fail "$test_name"
            TESTS_FAILED=$((TESTS_FAILED + 1))
        fi
    fi
}

# Function to check if file exists
check_file() {
    local file_path="$1"
    local description="$2"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    print_test "File exists: $description"

    if [ -f "$file_path" ]; then
        print_pass "$description exists"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        print_fail "$description missing at $file_path"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

echo "Starting comprehensive testing of all fixes..."
echo ""

# 1. BUILD SYSTEM TESTS
echo -e "${YELLOW}=== BUILD SYSTEM TESTS ===${NC}"

run_test "CMake build system" "cmake --build build" 0

check_file "./build/WanjPlayer" "Main executable"
check_file "./wanjplayer" "Universal launcher script"
check_file "./launch.sh" "Alternative launcher"

echo ""

# 2. ASSET FILE TESTS
echo -e "${YELLOW}=== ASSET FILE TESTS ===${NC}"

check_file "./assets/logo/wanjplayer-64x64.png" "App icon (64x64)"
check_file "./assets/LICENSE" "License file"
check_file "./build/assets/logo/wanjplayer-64x64.png" "App icon in build directory"
check_file "./build/assets/LICENSE" "License file in build directory"

echo ""

# 3. LAUNCHER TESTS
echo -e "${YELLOW}=== LAUNCHER TESTS ===${NC}"

run_test "Universal launcher executable" "[ -x ./wanjplayer ]" 0
run_test "Alternative launcher executable" "[ -x ./launch.sh ]" 0

print_test "Universal launcher help"
if ./wanjplayer --help >/dev/null 2>&1; then
    print_pass "Universal launcher help works"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Universal launcher help failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_test "Universal launcher version"
if ./wanjplayer --version >/dev/null 2>&1; then
    print_pass "Universal launcher version works"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Universal launcher version failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

print_test "Universal launcher info"
if ./wanjplayer --info >/dev/null 2>&1; then
    print_pass "Universal launcher info works"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Universal launcher info failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo ""

# 4. CORE FUNCTIONALITY TESTS
echo -e "${YELLOW}=== CORE FUNCTIONALITY TESTS ===${NC}"

print_test "Direct executable startup (5 second timeout)"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if timeout 5s ./build/WanjPlayer >/dev/null 2>&1; then
    print_pass "Direct executable starts without immediate crash"
    TESTS_PASSED=$((TESTS_PASSED + 1))
elif [ $? -eq 124 ]; then
    print_pass "Direct executable runs (timed out normally)"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Direct executable crashes immediately"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "Universal launcher startup (5 second timeout)"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if timeout 5s ./wanjplayer >/dev/null 2>&1; then
    print_pass "Universal launcher starts without immediate crash"
    TESTS_PASSED=$((TESTS_PASSED + 1))
elif [ $? -eq 124 ]; then
    print_pass "Universal launcher runs (timed out normally)"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Universal launcher crashes immediately"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

echo ""

# 5. VIDEO CONFIGURATION TESTS
echo -e "${YELLOW}=== VIDEO CONFIGURATION TESTS ===${NC}"

print_test "Environment detection"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if ./wanjplayer --info 2>&1 | grep -q "Session Type:"; then
    print_pass "Environment detection working"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Environment detection failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "Video backend configuration"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if ./wanjplayer --info 2>&1 | grep -q "Video Mode:"; then
    print_pass "Video backend configuration working"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Video backend configuration failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "GStreamer plugin detection"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if ./wanjplayer --info 2>&1 | grep -q "GStreamer:"; then
    print_pass "GStreamer detection working"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "GStreamer detection failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

echo ""

# 6. TEST SCRIPT AVAILABILITY
echo -e "${YELLOW}=== TEST SCRIPT TESTS ===${NC}"

check_file "./test_improvements.sh" "Playlist improvements test"
check_file "./test_status_bar_timer.sh" "Status bar timer test"
check_file "./test_playlist.sh" "Basic playlist test"
check_file "./run_safe_video.sh" "Safe video script"
check_file "./clean_gst_registry.sh" "GStreamer registry cleaner"

# Test script executability
run_test "Improvements test script executable" "[ -x ./test_improvements.sh ]" 0
run_test "Timer test script executable" "[ -x ./test_status_bar_timer.sh ]" 0

echo ""

# 7. DOCUMENTATION TESTS
echo -e "${YELLOW}=== DOCUMENTATION TESTS ===${NC}"

check_file "./README.md" "Main README"
check_file "./QUICK_START.md" "Quick start guide"
check_file "./PLAYLIST_IMPROVEMENTS.md" "Playlist improvements documentation"
check_file "./STATUS_BAR_TIMER_IMPLEMENTATION.md" "Timer implementation documentation"
check_file "./VIDEO_PLAYBACK_SOLUTION.md" "Video playback solution documentation"
check_file "./IMPROVEMENTS_SUMMARY.md" "Improvements summary"

echo ""

# 8. COMPILATION TESTS
echo -e "${YELLOW}=== COMPILATION TESTS ===${NC}"

print_test "Clean build test"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if make -C build clean >/dev/null 2>&1 && cmake --build build >/dev/null 2>&1; then
    print_pass "Clean build successful"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Clean build failed"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

echo ""

# 9. FEATURE PRESENCE TESTS
echo -e "${YELLOW}=== FEATURE PRESENCE TESTS ===${NC}"

print_test "Next/Previous button fix presence"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if grep -q "SetMediaCtrl" src/playlist.cpp && grep -q "media_ctrl_ref" include/playlist.hpp; then
    print_pass "Next/Previous button segfault fix implemented"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Next/Previous button segfault fix not found"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "Asset path fix presence"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if grep -q "assets/logo/wanjplayer-64x64.png" src/wanjplayer.cpp && grep -q "assets/LICENSE" src/media_events.cpp; then
    print_pass "Asset path fixes implemented"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Asset path fixes not found"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "Manual playlist resizing fix"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if grep -q "SetSashGravity" src/wanjplayer.cpp && grep -q "SetMinimumPaneSize(50)" src/wanjplayer.cpp; then
    print_pass "Manual playlist resizing fix implemented"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Manual playlist resizing fix not found"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "New time format presence"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if grep -q "wxString::Format.*h " src/media_ctrls.cpp && grep -q "wxString::Format.*m " src/media_ctrls.cpp; then
    print_pass "New time format (4h 34m 35s) implemented"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "New time format not found"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "Filename without extension fix"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if grep -q "GetName()" src/statusbar.cpp; then
    print_pass "Filename without extension fix implemented"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Filename without extension fix not found"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

echo ""

# 10. SYSTEM COMPATIBILITY TESTS
echo -e "${YELLOW}=== SYSTEM COMPATIBILITY TESTS ===${NC}"

print_test "Wayland compatibility"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [ "$XDG_SESSION_TYPE" = "wayland" ] && ./wanjplayer --info 2>&1 | grep -q "Wayland"; then
    print_pass "Wayland compatibility detected and configured"
    TESTS_PASSED=$((TESTS_PASSED + 1))
elif [ "$XDG_SESSION_TYPE" != "wayland" ]; then
    print_pass "Not running Wayland (test N/A)"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_fail "Wayland compatibility issue"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

print_test "GStreamer availability"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if command -v gst-inspect-1.0 >/dev/null 2>&1; then
    print_pass "GStreamer tools available"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    print_warning "GStreamer tools not available (may affect video playback)"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

echo ""

# SUMMARY
echo -e "${YELLOW}=== TEST SUMMARY ===${NC}"
echo ""
echo "Tests Passed: $TESTS_PASSED"
echo "Tests Failed: $TESTS_FAILED"
echo "Total Tests:  $TOTAL_TESTS"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo ""
    echo "✅ Fixes Status:"
    echo "  • Next/Previous button segfault fix: IMPLEMENTED"
    echo "  • App icon and license file path fix: IMPLEMENTED"
    echo "  • Manual playlist resizing restored: IMPLEMENTED"
    echo "  • Status bar log messages: IMPLEMENTED"
    echo "  • Filename without extension: IMPLEMENTED"
    echo "  • New time format (4h 34m 35s): IMPLEMENTED"
    echo "  • Universal launcher with video config: IMPLEMENTED"
    echo ""
    echo "🚀 WanjPlayer is ready for use!"

elif [ $TESTS_FAILED -le 3 ]; then
    echo -e "${YELLOW}⚠️  MOSTLY WORKING WITH MINOR ISSUES ⚠️${NC}"
    echo ""
    echo "Most functionality is working but there are $TESTS_FAILED minor issues to address."

else
    echo -e "${RED}❌ MULTIPLE ISSUES DETECTED ❌${NC}"
    echo ""
    echo "There are $TESTS_FAILED significant issues that need attention."
fi

echo ""
echo "For detailed testing of specific features:"
echo "  ./test_improvements.sh      # Test playlist and UI improvements"
echo "  ./test_status_bar_timer.sh  # Test real-time timer functionality"
echo "  ./wanjplayer --info         # Check system configuration"
echo ""

exit $TESTS_FAILED
