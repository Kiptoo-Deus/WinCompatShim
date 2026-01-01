#include <stdio.h>
#include <string.h>

extern int test_version();
extern int test_dwm();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Running all tests...\n\n");
        int failures = 0;
        failures += test_version();
        failures += test_dwm();
        printf("\n%s\n", failures == 0 ? "All tests passed!" : "Some tests failed!");
        return failures;
    }

    if (strcmp(argv[1], "version") == 0) {
        return test_version();
    }
    else if (strcmp(argv[1], "dwm") == 0) {
        return test_dwm();
    }

    printf("Unknown test: %s\n", argv[1]);
    return 1;
}
