#include <Windows.h>
#include "WinCompatShim.h"

const wchar_t CLASS_NAME[] = L"RoundedWindowDemoClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUND);
        WinCompat_DwmSetSystemBackdropType(hwnd, DWMSBT_MAINWINDOW);
        return 0;

    case WM_SIZE:
        WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUND);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        HBRUSH brush = CreateSolidBrush(RGB(30, 30, 30));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));

        WINCOMPAT_OS_VERSION ver;
        WinCompat_GetOSVersion(&ver);

        wchar_t text[256];
        wsprintfW(text, L"WinCompatShim Demo\n\nOS Build: %lu\nWindows 11: %s\n\nRounded corners via polyfill!",
            ver.build,
            WinCompat_IsWindows11OrGreater() ? L"Yes (native)" : L"No (emulated)");

        RECT textRect = rect;
        textRect.top += 50;
        DrawTextW(hdc, text, -1, &textRect, DT_CENTER | DT_WORDBREAK);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        else if (wParam == '1') {
            WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUND);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (wParam == '2') {
            WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_ROUNDSMALL);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (wParam == '3') {
            WinCompat_DwmSetWindowCornerPreference(hwnd, DWMWCP_DONOTROUND);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        CLASS_NAME,
        L"WinCompatShim - Rounded Window Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
