#include <Windows.h>
#include <cstdio>
#include <fileapi.h>
#include <iostream>
#include <cstdbool>

/*
* Requirements
* Windows 10 19H1 or later, Windows 11 (22000 or later)
* Application manifests with UTF-8 support configuration
* /utf-8 flag
* -A functions
* and your will!
*/

bool ConfigureCodePage(HANDLE hInput, HANDLE hOutput);
void TestUtf16ConsoleIo(HANDLE hInput, HANDLE hOutput);
void TestUtf8ConsoleIo(HANDLE hInput, HANDLE hOutput);
void TestUTF8IoCrt();
void TestUTF8IoCpp();

// More driver functions
// Plan: UTF-8 normalization fix, UTF-8 for GUI, GitHub Actions
