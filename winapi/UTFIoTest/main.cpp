#include <Windows.h>
#include <cstdio>
#include <memory>
#include <fileapi.h>
#include <iostream>

#include "utf8io.hpp"
using namespace std;

// UTF-8 main with Windows API; We can use the old-school way..
// To show grapheme characters (Compounded character with zero-width joiner character) such as 🐻 in Windows Terminal,
// You should update it to 1.22 or later.
// Source: https://devblogs.microsoft.com/commandline/windows-terminal-preview-1-22-release/
int main(int argc, char** argv)
{
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    if (hOutput == INVALID_HANDLE_VALUE || hInput == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error: GetStdHandle() failed\n";
        return 1;
    }
    if (!ConfigureCodePage(hInput, hOutput))
    {
        std::cerr << "Error: Failed to configure code page.\n";
        return 1;
    }

    TestUTF8IoCrt();
    TestUTF8IoCpp();
    TestUtf8ConsoleIo(hInput, hOutput);
    TestUtf16ConsoleIo(hInput, hOutput);
    return 0;
}
