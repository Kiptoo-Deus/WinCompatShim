#pragma once

#ifdef WINCOMPATSHIM_EXPORTS
#define WINCOMPAT_API __declspec(dllexport)
#else
#define WINCOMPAT_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <dwmapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__MINGW32__) && defined(NTDDI_WIN10_NI) && (NTDDI_VERSION >= NTDDI_WIN10_NI)
#define WINCOMPAT_HAS_DWM_CORNER_PREFERENCE 1
#define WINCOMPAT_HAS_DWM_BACKDROP_TYPE 1
#else
#define WINCOMPAT_HAS_DWM_CORNER_PREFERENCE 0
#define WINCOMPAT_HAS_DWM_BACKDROP_TYPE 0
#endif

#if !WINCOMPAT_HAS_DWM_CORNER_PREFERENCE
#define DWMWA_WINDOW_CORNER_PREFERENCE 33
typedef enum WINCOMPAT_DWM_WINDOW_CORNER_PREFERENCE {
    DWMWCP_DEFAULT = 0,
    DWMWCP_DONOTROUND = 1,
    DWMWCP_ROUND = 2,
    DWMWCP_ROUNDSMALL = 3
} DWM_WINDOW_CORNER_PREFERENCE;
#endif

#if !WINCOMPAT_HAS_DWM_BACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
typedef enum WINCOMPAT_DWM_SYSTEMBACKDROP_TYPE {
    DWMSBT_AUTO = 0,
    DWMSBT_NONE = 1,
    DWMSBT_MAINWINDOW = 2,
    DWMSBT_TRANSIENTWINDOW = 3,
    DWMSBT_TABBEDWINDOW = 4
} DWM_SYSTEMBACKDROP_TYPE;
#endif

typedef struct {
    DWORD major;
    DWORD minor;
    DWORD build;
} WINCOMPAT_OS_VERSION;

WINCOMPAT_API BOOL WinCompat_GetOSVersion(WINCOMPAT_OS_VERSION* version);
WINCOMPAT_API BOOL WinCompat_IsWindows11OrGreater(void);
WINCOMPAT_API BOOL WinCompat_IsWindows11Build22H2OrGreater(void);
WINCOMPAT_API DWORD WinCompat_GetBuildNumber(void);

WINCOMPAT_API HRESULT WinCompat_DwmSetWindowCornerPreference(HWND hwnd, DWM_WINDOW_CORNER_PREFERENCE preference);
WINCOMPAT_API HRESULT WinCompat_DwmSetSystemBackdropType(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdropType);
WINCOMPAT_API HRESULT WinCompat_DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);

WINCOMPAT_API HRESULT WinCompat_EnableBlurBehind(HWND hwnd, BOOL enable);
WINCOMPAT_API HRESULT WinCompat_SetWindowRoundedCorners(HWND hwnd, int cornerRadius);

#ifdef __cplusplus
}
#endif
