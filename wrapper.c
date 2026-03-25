#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: wrapper <exe> [args...]\n");
        return 1;
    }

    // Build command line
    char cmdline[4096] = {0};

    strcat(cmdline, "\"");
    strcat(cmdline, argv[1]);
    strcat(cmdline, "\"");

    for (int i = 2; i < argc; ++i) {
        strcat(cmdline, " ");
        if (strchr(argv[i], ' ')) {
            strcat(cmdline, "\"");
            strcat(cmdline, argv[i]);
            strcat(cmdline, "\"");
        } else {
            strcat(cmdline, argv[i]);
        }
    }

    // Print invocation
    printf("Executing: %s\n", cmdline);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(
            NULL,
            cmdline,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi)) {
        perror("CreateProcess failed");
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return exitCode;
}
