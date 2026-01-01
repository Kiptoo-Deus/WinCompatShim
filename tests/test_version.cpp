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

int test_version() {
    int failures = 0;
    printf("Version Tests:\n");

    WINCOMPAT_OS_VERSION ver;
    BOOL result = WinCompat_GetOSVersion(&ver);
    TEST("WinCompat_GetOSVersion returns TRUE", result == TRUE);
    TEST("Major version is 10", ver.major == 10);
    TEST("Build number is reasonable", ver.build >= 17763);

    DWORD build = WinCompat_GetBuildNumber();
    TEST("WinCompat_GetBuildNumber returns non-zero", build > 0);
    TEST("Build numbers match", build == ver.build);

    BOOL isWin11 = WinCompat_IsWindows11OrGreater();
    TEST("IsWindows11OrGreater matches build check", isWin11 == (build >= 22000));

    BOOL isWin11_22H2 = WinCompat_IsWindows11Build22H2OrGreater();
    TEST("IsWindows11Build22H2OrGreater matches build check", isWin11_22H2 == (build >= 22621));

    TEST("WinCompat_GetOSVersion handles NULL", WinCompat_GetOSVersion(NULL) == FALSE);

    printf("Version tests: %d failures\n", failures);
    return failures;
}
