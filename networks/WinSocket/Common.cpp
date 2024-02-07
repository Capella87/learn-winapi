#include "Common.hpp"

// Show socket error and terminate program
void err_quitW(LPCWSTR msg)
{
    LPVOID lpMsgBuf;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
               NULL, WSAGetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (LPWSTR)&lpMsgBuf, 0, NULL);
    MessageBoxW(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);

    exit(1);
}

// LPCWSTR = typedef const wchar_t*
// Print socket error by msg
void err_displayW(LPCWSTR msg)
{
    LPVOID lpMsgBuf;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lpMsgBuf, 0, NULL);
    wprintf(L"[%s] %s\n", msg, (LPWSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

// Print Socket error by error code
void err_displayW(int errcode)
{
    LPVOID lpMsgBuf;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lpMsgBuf, 0, NULL);
    wprintf(L"[Error] %s\n", (LPWSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
