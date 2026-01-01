#include "WinCompatShim.h"
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")

static HRGN CreateRoundedRegion(HWND hwnd, int cornerRadius) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    return CreateRoundRectRgn(0, 0, width + 1, height + 1, cornerRadius, cornerRadius);
}

WINCOMPAT_API HRESULT WinCompat_SetWindowRoundedCorners(HWND hwnd, int cornerRadius) {
    if (!hwnd || !IsWindow(hwnd)) return E_INVALIDARG;

    HRGN hRgn = CreateRoundedRegion(hwnd, cornerRadius);
    if (!hRgn) return E_FAIL;

    if (SetWindowRgn(hwnd, hRgn, TRUE) == 0) {
        DeleteObject(hRgn);
        return E_FAIL;
    }

    return S_OK;
}

WINCOMPAT_API HRESULT WinCompat_EnableBlurBehind(HWND hwnd, BOOL enable) {
    if (!hwnd || !IsWindow(hwnd)) return E_INVALIDARG;

    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = enable;
    bb.hRgnBlur = NULL;

    if (enable) {
        bb.dwFlags |= DWM_BB_BLURREGION;
        RECT rect;
        GetClientRect(hwnd, &rect);
        bb.hRgnBlur = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    }

    HRESULT hr = DwmEnableBlurBehindWindow(hwnd, &bb);

    if (bb.hRgnBlur) {
        DeleteObject(bb.hRgnBlur);
    }

    return hr;
}

WINCOMPAT_API HRESULT WinCompat_DwmSetWindowCornerPreference(HWND hwnd, DWM_WINDOW_CORNER_PREFERENCE preference) {
    if (!hwnd || !IsWindow(hwnd)) return E_INVALIDARG;

    if (WinCompat_IsWindows11OrGreater()) {
        DWORD pref = (DWORD)preference;
        return DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &pref, sizeof(pref));
    }

    switch (preference) {
    case DWMWCP_ROUND:
        return WinCompat_SetWindowRoundedCorners(hwnd, 20);
    case DWMWCP_ROUNDSMALL:
        return WinCompat_SetWindowRoundedCorners(hwnd, 8);
    case DWMWCP_DONOTROUND:
    case DWMWCP_DEFAULT:
    default:
        SetWindowRgn(hwnd, NULL, TRUE);
        return S_OK;
    }
}

WINCOMPAT_API HRESULT WinCompat_DwmSetSystemBackdropType(HWND hwnd, DWM_SYSTEMBACKDROP_TYPE backdropType) {
    if (!hwnd || !IsWindow(hwnd)) return E_INVALIDARG;

    if (WinCompat_IsWindows11Build22H2OrGreater()) {
        DWORD type = (DWORD)backdropType;
        return DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &type, sizeof(type));
    }

    switch (backdropType) {
    case DWMSBT_MAINWINDOW:
    case DWMSBT_TRANSIENTWINDOW:
    case DWMSBT_TABBEDWINDOW:
        return WinCompat_EnableBlurBehind(hwnd, TRUE);
    case DWMSBT_NONE:
    case DWMSBT_AUTO:
    default:
        return WinCompat_EnableBlurBehind(hwnd, FALSE);
    }
}

WINCOMPAT_API HRESULT WinCompat_DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute) {
    if (!hwnd || !IsWindow(hwnd)) return E_INVALIDARG;
    if (!pvAttribute) return E_INVALIDARG;

    switch (dwAttribute) {
    case DWMWA_WINDOW_CORNER_PREFERENCE:
        if (cbAttribute >= sizeof(DWORD)) {
            return WinCompat_DwmSetWindowCornerPreference(hwnd, (DWM_WINDOW_CORNER_PREFERENCE)(*(DWORD*)pvAttribute));
        }
        return E_INVALIDARG;

    case DWMWA_SYSTEMBACKDROP_TYPE:
        if (cbAttribute >= sizeof(DWORD)) {
            return WinCompat_DwmSetSystemBackdropType(hwnd, (DWM_SYSTEMBACKDROP_TYPE)(*(DWORD*)pvAttribute));
        }
        return E_INVALIDARG;

    default:
        return DwmSetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute);
    }
}
