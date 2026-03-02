#!/bin/bash
#
# build.sh
#
# One-shot build script for HPSS.
# Handles vcpkg setup, dependency resolution, CMake configuration, and build.
#
# Usage:
#   ./build.sh           # Release build
#   ./build.sh debug     # Debug build
#   ./build.sh clean     # Remove build artifacts
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

BUILD_TYPE="${1:-Release}"

# clean subcommand
if [ "$BUILD_TYPE" = "clean" ]; then
    echo "Removing build artifacts..."
    rm -rf build
    echo "Done."
    exit 0
fi

# debug subcommand
if [ "$BUILD_TYPE" = "debug" ]; then
    BUILD_TYPE="Debug"
fi

# Install cmake if not found
if ! command -v cmake &> /dev/null; then
    echo "CMake not found. Installing..."
    if command -v brew &> /dev/null; then
        brew install cmake
    elif command -v apt-get &> /dev/null; then
        sudo apt-get update && sudo apt-get install -y cmake
    else
        echo "Error: cannot install cmake automatically. Please install it manually."
        exit 1
    fi
fi

# Set up vcpkg (clone only if not present)
if [ ! -d "vcpkg" ]; then
    echo "Cloning vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git
fi

if [ ! -f "vcpkg/vcpkg" ] && [ ! -f "vcpkg/vcpkg.exe" ]; then
    echo "Bootstrapping vcpkg..."
    ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi

# CMake configure + build
echo "Build type: ${BUILD_TYPE}"
cmake --preset default -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build build

echo ""
echo "Build complete:"
echo "  build/HPSS_separation"
echo "  build/HPSS_smoothness_eval"
