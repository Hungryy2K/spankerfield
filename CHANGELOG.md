# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [2.2.0] - 2025-07-28
### Added
- Enhanced ESP features with improved bone drawing and 3D box visualization
- Advanced anti-cheat stealth mechanisms with process detection evasion
- Modern UI improvements with new color definitions and ESP checkboxes
- Performance optimizations for thread pool and memory management
- New ESP settings for radar, hit markers, bullet tracers, and visibility indicators
- Enhanced weapon indicators and health bar 3D visualization

### Changed
- Updated project configuration to resolve compiler flag conflicts
- Improved JSON deserialization to handle null values in settings
- Enhanced GUI with new checkboxes for advanced ESP features
- Optimized build settings for better compilation performance

### Fixed
- Resolved all compiler warnings (D9025) by removing conflicting optimization flags
- Fixed `volatile` qualifier errors in blacklist.cpp
- Corrected function declarations in gui.h by removing default arguments
- Fixed static member definitions in thread_pool.cpp
- Added missing Windows API includes for sound playback functionality
- Resolved ImVec2 arithmetic errors in tooltip.h

### Technical Details
- Set proper optimization levels (`MinSpace`) and inline function expansion (`AnySuitable`)
- Disabled frame pointer omission (`OmitFramePointers: false`) to prevent conflicts
- Improved build configuration for Release|x64 and Release|Win32 targets
- Enhanced code quality with better error handling and type safety

## [2.1.0] - 2025-03-03
### Added
- 3D box ESP for players and vehicles (47b3ea6)
- Eye tracer functionality for displaying player's view direction (47b3ea6)
- Improved ImGui color picker with support for alpha channel and preview (67d3eab)
- Extended GUI settings for new ESP features (47b3ea6)
- Additional PBSS cleaner method (67112a3)
- Customizable PBSS delay (1ccb9ed)
- Increased post pb delay (a0b03e6)

### Changed
- Completely rewritten screenshot cleaners (b3b98f4)
- Prepared gitignore for cmake (5d5785e)
- Added bat script for developer prompt (5d5785e)

### Fixed
- Minor fixes in PBSS (18c8f67)
- Modifications in aimbot.cpp (be5c4b6)

## [2.0.1] - 2025-02-20
### Added
- Tooltip formatting with color support (8a36cfb52a1ec7ae37c300e2e228fba58595af42)
- Weapon untested functions from SDK
### Fixed
- SDK offset padding for weapon fireing

## [2.0.0] - 2025-02-19
### Added
- Zero point correction system for aiming (a4c2a57)
- Enhanced recoil and spread settings (8800141)
- Advanced bone selection and targeting options for aimbot (4a2b6b1)
- Reorganized GUI tabs for better navigation (a4c2a57)
### Fixed
- Improved aimbot prediction algorithm (6dc1ce4)

## [1.2.0] - 2025-01-10
### Added
- Infantry alert indicators (f53d8a0)
### Fixed
- ImGui errors resolved (f53d8a0)
- Aimbot improvements (f53d8a0)

## [1.1.0] - 2024-07
### Added
- Radar icon scaling (0293534)
- No spread functionality (9ff7cdb)
- Reload check implementation (9ff7cdb)
- Rainbow speed customization (c25e174)
### Fixed
- No recoil crash fixes (ba06a3c)
- Updated renderer and ImGui (7e59015)
- Updated fonts (7e59015)

## [1.0.0] - 2024-06
### Added
- Controller support (2c4d176)
- Enhanced rainbow mode (2c4d176)
- Key binding customization (2c4d176)
- Skeleton color settings (visible/not visible) (ab5f0dc)
- Improved kill sound (3f628d4)
- Anti-AFK system (3f628d4)
- New PreFrame hooking system (620da56)
- Radar customization (e9f8bfb)

### Fixed
- Improved overall stability (874cf7c)
- Optimized codebase (56e5d56)
- Fixed disconnection crashes (5ba60da)
- Enhanced aimbot accuracy (d7a0748)

### Technical Details
- Modern C++20 implementation (56e5d56)
- Strict memory management practices (56e5d56)
- Structured codebase organization (56e5d56) 