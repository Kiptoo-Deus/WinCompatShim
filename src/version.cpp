#include "WinCompatShim.h"
#include <winternl.h>

typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

static BOOL GetRealOSVersion(RTL_OSVERSIONINFOW* osvi) {
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (!hNtdll) return FALSE;

    RtlGetVersionPtr rtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hNtdll, "RtlGetVersion");
    if (!rtlGetVersion) return FALSE;

    osvi->dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
    return rtlGetVersion(osvi) == 0;
}

WINCOMPAT_API BOOL WinCompat_GetOSVersion(WINCOMPAT_OS_VERSION* version) {
    if (!version) return FALSE;

    RTL_OSVERSIONINFOW osvi = { 0 };
    if (!GetRealOSVersion(&osvi)) return FALSE;

    version->major = osvi.dwMajorVersion;
    version->minor = osvi.dwMinorVersion;
    version->build = osvi.dwBuildNumber;
    return TRUE;
}

WINCOMPAT_API DWORD WinCompat_GetBuildNumber(void) {
    RTL_OSVERSIONINFOW osvi = { 0 };
    if (!GetRealOSVersion(&osvi)) return 0;
    return osvi.dwBuildNumber;
}

WINCOMPAT_API BOOL WinCompat_IsWindows11OrGreater(void) {
    return WinCompat_GetBuildNumber() >= 22000;
}

WINCOMPAT_API BOOL WinCompat_IsWindows11Build22H2OrGreater(void) {
    return WinCompat_GetBuildNumber() >= 22621;
}
