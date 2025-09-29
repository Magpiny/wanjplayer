#!/bin/bash

# WanjPlayer Setup Validation Script
# Author: Development Team
# Description: Validates that the development environment is properly configured

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
PASSED=0
FAILED=0
WARNINGS=0

# Function to print colored output
print_test() {
    echo -e "${BLUE}[TEST]${NC} $1"
}

print_pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
    ((PASSED++))
}

print_fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    ((FAILED++))
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
    ((WARNINGS++))
}

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

# Header
echo "========================================="
echo "WanjPlayer Development Environment Check"
echo "========================================="
echo ""

# Check if we're in the right directory
print_test "Checking project structure"
if [ -f "CMakeLists.txt" ] && [ -d "src" ] && [ -d "include" ]; then
    print_pass "Project structure is correct"
else
    print_fail "Not in WanjPlayer root directory or missing essential files"
    exit 1
fi

# Check required tools
print_test "Checking required development tools"

# Check cmake
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | grep -oP '\d+\.\d+\.\d+')
    print_pass "CMake found (version $CMAKE_VERSION)"
else
    print_fail "CMake not found"
fi

# Check compiler
if command -v g++ &> /dev/null; then
    GCC_VERSION=$(g++ --version | head -n1 | grep -oP '\d+\.\d+\.\d+' | head -n1)
    print_pass "GCC found (version $GCC_VERSION)"
else
    print_fail "GCC not found"
fi

# Check clangd
if command -v clangd &> /dev/null; then
    CLANGD_VERSION=$(clangd --version | head -n1 | grep -oP '\d+\.\d+\.\d+' | head -n1)
    print_pass "clangd found (version $CLANGD_VERSION)"
else
    print_fail "clangd not found - language server won't work properly"
fi

# Check clang-format
if command -v clang-format &> /dev/null; then
    CLANG_FORMAT_VERSION=$(clang-format --version | grep -oP '\d+\.\d+\.\d+')
    print_pass "clang-format found (version $CLANG_FORMAT_VERSION)"
else
    print_warn "clang-format not found - code formatting won't work"
fi

# Check clang-tidy
if command -v clang-tidy &> /dev/null; then
    CLANG_TIDY_VERSION=$(clang-tidy --version | grep -oP '\d+\.\d+\.\d+')
    print_pass "clang-tidy found (version $CLANG_TIDY_VERSION)"
else
    print_warn "clang-tidy not found - static analysis won't work"
fi

# Check wxWidgets
print_test "Checking wxWidgets installation"
if command -v wx-config &> /dev/null; then
    WX_VERSION=$(wx-config --version)
    WX_CPPFLAGS=$(wx-config --cppflags)
    print_pass "wxWidgets found (version $WX_VERSION)"
    print_info "wxWidgets flags: $WX_CPPFLAGS"
else
    print_fail "wx-config not found - wxWidgets not properly installed"
fi

# Check configuration files
print_test "Checking configuration files"

# Check .clangd
if [ -f ".clangd" ]; then
    print_pass ".clangd configuration file exists"
else
    print_warn ".clangd configuration file missing"
fi

# Check .clang-format
if [ -f ".clang-format" ]; then
    print_pass ".clang-format configuration file exists"
else
    print_warn ".clang-format configuration file missing"
fi

# Check .clang-tidy
if [ -f ".clang-tidy" ]; then
    print_pass ".clang-tidy configuration file exists"
else
    print_warn ".clang-tidy configuration file missing"
fi

# Check build directory and files
print_test "Checking build configuration"

if [ -d "build" ]; then
    print_pass "Build directory exists"

    if [ -f "build/compile_commands.json" ]; then
        print_pass "compile_commands.json exists in build directory"

        # Check if symlink exists in root
        if [ -L "compile_commands.json" ]; then
            if [ -f "compile_commands.json" ]; then
                print_pass "compile_commands.json symlink is valid"
            else
                print_fail "compile_commands.json symlink is broken"
            fi
        else
            print_warn "compile_commands.json symlink missing in root directory"
        fi
    else
        print_warn "compile_commands.json not found in build directory"
        print_info "Run './build.sh rebuild' to generate it"
    fi
else
    print_warn "Build directory doesn't exist"
    print_info "Run './build.sh' to create and configure it"
fi

# Test build
print_test "Testing build process"
if [ -f "build.sh" ] && [ -x "build.sh" ]; then
    print_pass "Build script is executable"

    # Try to configure if not already done
    if [ ! -f "build/CMakeCache.txt" ]; then
        print_info "Configuring project..."
        if ./build.sh configure &> /dev/null; then
            print_pass "Project configuration successful"
        else
            print_fail "Project configuration failed"
        fi
    else
        print_pass "Project already configured"
    fi

    # Try to build
    print_info "Testing build..."
    if ./build.sh build &> /dev/null; then
        print_pass "Build successful"

        # Check if executable was created
        if [ -f "build/WanjPlayer" ]; then
            print_pass "WanjPlayer executable created"
        else
            print_fail "WanjPlayer executable not found after build"
        fi
    else
        print_fail "Build failed"
    fi
else
    print_fail "Build script missing or not executable"
fi

# Check VS Code configuration
print_test "Checking VS Code configuration"
if [ -d ".vscode" ]; then
    print_pass ".vscode directory exists"

    if [ -f ".vscode/settings.json" ]; then
        print_pass "VS Code settings configured"
    else
        print_warn "VS Code settings.json missing"
    fi

    if [ -f ".vscode/tasks.json" ]; then
        print_pass "VS Code tasks configured"
    else
        print_warn "VS Code tasks.json missing"
    fi

    if [ -f ".vscode/launch.json" ]; then
        print_pass "VS Code launch configurations available"
    else
        print_warn "VS Code launch.json missing"
    fi

    if [ -f ".vscode/c_cpp_properties.json" ]; then
        print_pass "C/C++ properties configured"
    else
        print_warn "VS Code c_cpp_properties.json missing"
    fi
else
    print_warn ".vscode directory missing"
fi

# Test language server integration
print_test "Testing language server integration"
if [ -f "compile_commands.json" ] && command -v clangd &> /dev/null; then
    # Test clangd with a simple file
    if [ -f "src/wanjplayer.cpp" ]; then
        print_info "Testing clangd with wanjplayer.cpp..."
        if timeout 10s clangd --check=src/wanjplayer.cpp &> /dev/null; then
            print_pass "clangd can analyze source files"
        else
            print_warn "clangd analysis test inconclusive (this may be normal)"
        fi
    fi
else
    print_warn "Cannot test language server - missing compile_commands.json or clangd"
fi

# Summary
echo ""
echo "========================================="
echo "Validation Summary"
echo "========================================="
echo -e "${GREEN}Passed:${NC} $PASSED"
echo -e "${YELLOW}Warnings:${NC} $WARNINGS"
echo -e "${RED}Failed:${NC} $FAILED"
echo ""

if [ $FAILED -eq 0 ]; then
    if [ $WARNINGS -eq 0 ]; then
        echo -e "${GREEN}🎉 Perfect! Your development environment is fully configured.${NC}"
        echo ""
        echo "You can now:"
        echo "  • Use your IDE with full IntelliSense support"
        echo "  • Build with: ./build.sh"
        echo "  • Run with: ./build.sh run"
        echo "  • Format code with: ./build.sh format"
    else
        echo -e "${YELLOW}✅ Good! Your environment is mostly configured.${NC}"
        echo "Consider addressing the warnings above for the best experience."
    fi
else
    echo -e "${RED}❌ Some issues need to be resolved.${NC}"
    echo "Please fix the failed checks above before continuing."
    echo ""
    echo "Common solutions:"
    echo "  • Install missing packages: sudo apt install build-essential cmake clang clangd libwxgtk3.2-dev"
    echo "  • Run: ./build.sh rebuild"
    echo "  • Check the DEVELOPMENT.md file for detailed setup instructions"
    exit 1
fi

echo ""
echo "For more information, see docs/DEVELOPMENT.md"
