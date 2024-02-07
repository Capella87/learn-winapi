#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include <wchar.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32")

void err_quitW(LPCWSTR msg);
void err_displayW(LPCWSTR msg);
void err_displayW(int errcode);
