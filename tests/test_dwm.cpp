#include <Windows.h>
#include <stdio.h>
#include "WinCompatShim.h"

#define TEST(name, condition) do { \
    if (condition) { \
        printf("  [PASS] %s\n", name); \
    } else { \
        printf("  [FAIL] %s\n", name); \
        failures++; \
    } \
} while(0)

int test_dwm() {
    int failures = 0;
    printf("DWM Tests:\n");

    TEST("Corner preference with NULL hwnd returns E_INVALIDARG",
        WinCompat_DwmSetWindowCornerPreference(NULL, DWMWCP_ROUND) == E_INVALIDARG);

    TEST("Backdrop type with NULL hwnd returns E_INVALIDARG",
        WinCompat_DwmSetSystemBackdropType(NULL, DWMSBT_MAINWINDOW) == E_INVALIDARG);

    TEST("SetWindowAttribute with NULL hwnd returns E_INVALIDARG",
        WinCompat_DwmSetWindowAttribute(NULL, DWMWA_WINDOW_CORNER_PREFERENCE, NULL, 0) == E_INVALIDARG);

    TEST("EnableBlurBehind with NULL hwnd returns E_INVALIDARG",
        WinCompat_EnableBlurBehind(NULL, TRUE) == E_INVALIDARG);

    TEST("SetWindowRoundedCorners with NULL hwnd returns E_INVALIDARG",
        WinCompat_SetWindowRoundedCorners(NULL, 10) == E_INVALIDARG);

    HWND hwnd = CreateWindowExW(
        0, L"STATIC", L"Test",
        WS_OVERLAPPEDWINDOW,
        0, 0, 100, 100,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    if (hwnd) {
        TEST("Corner preference ROUND succeeds",
            SUCCEEDED(WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUND)));

        TEST("Corner preference ROUNDSMALL succeeds",
            SUCCEEDED(WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUNDSMALL)));

        TEST("Corner preference DONOTROUND succeeds",
            SUCCEEDED(WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_DONOTROUND)));

        TEST("SetWindowRoundedCorners succeeds",
            SUCCEEDED(WinCompat_SetWindowRoundedCorners(hwnd, 15)));

        DWORD pref = DWMWCP_ROUND;
        TEST("DwmSetWindowAttribute for corners succeeds",
            SUCCEEDED(WinCompat_DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &pref, sizeof(pref))));

        DestroyWindow(hwnd);
    }
    else {
        printf("  [SKIP] Window creation tests (no window)\n");
    }

    printf("DWM tests: %d failures\n", failures);
    return failures;
}
