# WinCompatShim

Cross-version Windows API shims / polyfills library. Provides wrappers for Windows APIs introduced in newer versions (Windows 11+) with automatic fallbacks for older systems (Windows 10).

## Features

- **OS Version Detection**: Safe wrapper around `RtlGetVersion` for accurate version checks (bypasses manifest-based lies)
- **DWM Window Corner Preference**: Native support on Win11, emulated via `SetWindowRgn` on Win10
- **DWM System Backdrop Type**: Mica/Acrylic on Win11 22H2+, blur-behind fallback on older systems
- **Drop-in Replacement**: Use `WinCompat_DwmSetWindowAttribute` as a replacement for direct API calls
- **Zero External Dependencies**: Uses only Windows SDK

## Supported APIs

| API | Win11+ | Win10 Fallback |
|-----|--------|----------------|
| `DWMWA_WINDOW_CORNER_PREFERENCE` | Native | `SetWindowRgn` with rounded region |
| `DWMWA_SYSTEMBACKDROP_TYPE` | Native (22H2+) | `DwmEnableBlurBehindWindow` |

## Building

### Requirements
- CMake 3.15+
- Visual Studio 2019+ or compatible compiler
- Windows SDK 10.0.17763+

### Build Steps

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Build Options

- `BUILD_EXAMPLES=ON/OFF` - Build demo applications (default: ON)
- `BUILD_TESTS=ON/OFF` - Build test suite (default: ON)

## Usage

### Link to your project

```cmake
find_package(WinCompatShim REQUIRED)
target_link_libraries(YourApp PRIVATE WinCompat::WinCompatShim)
```

### Basic Example

```cpp
#include <WinCompatShim.h>

// Set rounded corners (works on Win10 and Win11)
WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUND);

// Apply Mica effect (Mica on Win11 22H2+, blur on older)
WinCompat_DwmSetSystemBackdropType(hwnd, DWMSBT_MAINWINDOW);

// Check OS version
WINCOMPAT_OS_VERSION ver;
WinCompat_GetOSVersion(&ver);
printf("Build: %lu\n", ver.build);

// Or use convenience functions
if (WinCompat_IsWindows11OrGreater()) {
    // Native Win11 features available
}
```

### Drop-in Replacement

Replace direct `DwmSetWindowAttribute` calls:

```cpp
// Before (crashes or fails on Win10 for new attributes)
DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &pref, sizeof(pref));

// After (works everywhere)
WinCompat_DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &pref, sizeof(pref));
```

## API Reference

### Version Detection

```cpp
BOOL WinCompat_GetOSVersion(WINCOMPAT_OS_VERSION* version);
DWORD WinCompat_GetBuildNumber(void);
BOOL WinCompat_IsWindows11OrGreater(void);
BOOL WinCompat_IsWindows11Build22H2OrGreater(void);
```

### DWM Shims

```cpp
HRESULT WinCompat_DwmSetWindowCornerPreference(HWND hwnd, DWM_WINDOW_CORNER_PREFERENCE preference);
HRESULT WinCompat_DwmSetSystemBackdropType(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdropType);
HRESULT WinCompat_DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
```

### Utility Functions

```cpp
HRESULT WinCompat_EnableBlurBehind(HWND hwnd, BOOL enable);
HRESULT WinCompat_SetWindowRoundedCorners(HWND hwnd, int cornerRadius);
```

## Demo Application

Run the included demo to see rounded corners in action:

```
build\examples\Release\RoundedWindowDemo.exe
```

- Press `1` - Large rounded corners
- Press `2` - Small rounded corners  
- Press `3` - Square corners
- Press `ESC` - Exit

## License

MIT License - see [LICENSE](LICENSE) file.

## Contributing

Contributions welcome! Areas for expansion:
- Additional DWM attributes
- Memory management APIs
- Security APIs
- Accessibility APIs
