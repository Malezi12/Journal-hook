#include <windows.h>
#include <stdio.h>
#include <string>
#include "include/detours.h"

static HANDLE(WINAPI* TrueCreateFileW)(
    LPCWSTR, DWORD, DWORD,
    LPSECURITY_ATTRIBUTES, DWORD,
    DWORD, HANDLE) = CreateFileW;

HANDLE WINAPI HookedCreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    if (lpFileName &&
        (wcsstr(lpFileName, L"MEM REDUCT.exe") ||
            wcsstr(lpFileName, L"MEM REDUCT.EXE") ||
            wcsstr(lpFileName, L"UnRAR.exe") ||
            wcsstr(lpFileName, L"UnRAR") ||
            wcsstr(lpFileName, L"bandicam.dll") ||
            wcsstr(lpFileName, L"bandicam")))
    {
        FILE* logFile;
        _wfopen_s(&logFile, L"C:\\Temp\\FileHookLog.txt", L"a");
        if (logFile) {
            fwprintf(logFile, L"%s\n", lpFileName);
            fclose(logFile);
        }

        return INVALID_HANDLE_VALUE;
    }

    return TrueCreateFileW(
        lpFileName, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition,
        dwFlagsAndAttributes, hTemplateFile);
}

BOOL SetupHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((PVOID*)&TrueCreateFileW, HookedCreateFileW);
    return DetourTransactionCommit() == NO_ERROR;
}

BOOL RemoveHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach((PVOID*)&TrueCreateFileW, HookedCreateFileW);
    return DetourTransactionCommit() == NO_ERROR;
}

BOOL APIENTRY DllMain(HMODULE h, DWORD r, LPVOID) {
    if (r == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(h);
        return SetupHook();
    }
    else if (r == DLL_PROCESS_DETACH)
        return RemoveHook();
    return TRUE;
}
