# CMakeLists.txt
#
# HPSS (Harmonic/Percussive Sound Separation) build configuration
#
# Build (with vcpkg):
#   cmake --preset default
#   cmake --build build
#
# Build (without vcpkg, using system libraries):
#   cmake -B build
#   cmake --build build
#
# Debug build:
#   cmake --preset default -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build
#

cmake_minimum_required(VERSION 3.15)

# Handle FindBoost module removal (CMake 3.30+)
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(HPSS
    VERSION 1.0.0
    DESCRIPTION "Harmonic/Percussive Sound Separation"
    LANGUAGES CXX
)

# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

# Find dependencies
# Works with both vcpkg and system-installed libraries
find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
find_package(Threads REQUIRED)

# FFTW3: try CONFIG mode first (vcpkg provides this)
find_package(FFTW3 CONFIG QUIET)

if(NOT FFTW3_FOUND)
    # Fallback for system-installed FFTW3: pkg-config -> manual search
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(FFTW3 QUIET fftw3)
    endif()

    if(NOT FFTW3_FOUND)
        find_path(FFTW3_INCLUDE_DIRS fftw3.h)
        find_library(FFTW3_LIBRARIES NAMES fftw3)
        if(FFTW3_INCLUDE_DIRS AND FFTW3_LIBRARIES)
            set(FFTW3_FOUND TRUE)
        else()
            message(FATAL_ERROR
                "FFTW3 not found.\n"
                "  With vcpkg: run 'cmake --preset default' to install automatically\n"
                "  Manual: install libfftw3-dev (Ubuntu) or fftw (Homebrew)"
            )
        endif()
    endif()
endif()

# Unify FFTW3 link target
# vcpkg provides FFTW3::fftw3 target; system install uses variables
if(TARGET FFTW3::fftw3)
    set(FFTW3_LINK_TARGET FFTW3::fftw3)
else()
    set(FFTW3_LINK_TARGET ${FFTW3_LIBRARIES})
endif()

# Shared source files
set(HPSS_SHARED_SOURCES
    src/HPSS.cpp
    src/STFT.cpp
    src/invSTFT.cpp
    src/twoDimArray.cpp
    src/objFunction.cpp
    src/waviostream.cpp
    src/HPSS_specific.cpp
)

# Executable: HPSS_separation
add_executable(HPSS_separation
    src/HPSS_separation.cpp
    ${HPSS_SHARED_SOURCES}
)

# Executable: HPSS_smoothness_eval
add_executable(HPSS_smoothness_eval
    src/HPSS_smoothness_eval.cpp
    ${HPSS_SHARED_SOURCES}
)

# Apply common settings to both executables
foreach(target HPSS_separation HPSS_smoothness_eval)
    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${FFTW3_INCLUDE_DIRS}
    )
    target_link_libraries(${target} PRIVATE
        Boost::program_options
        ${FFTW3_LINK_TARGET}
        Threads::Threads
        m
    )
endforeach()
