#include "utf8io.hpp"

#include <memory>
using namespace std;

bool ConfigureCodePage(HANDLE hInput, HANDLE hOutput)
{
    UINT codePage = GetConsoleOutputCP();
    if (codePage != CP_UTF8)
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        DWORD dwMode = 0;
        GetConsoleMode(hOutput, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        SetConsoleMode(hOutput, dwMode);

        dwMode = 0;
        GetConsoleMode(hInput, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_PROCESSED_INPUT;
        SetConsoleMode(hInput, dwMode);
    }
    return true;
}

void TestUtf16ConsoleIo(HANDLE hInput, HANDLE hOutput)
{
    WriteConsoleW(hOutput, L"\x1b[34mThe old way with Windows API!\x1b[0m\n", 39, NULL, NULL);
    WriteConsoleW(hOutput, L"아무 문자나 입력해 주세요: ", 16, NULL, NULL);

    // Classical UTF-16 way
    // We should use wchar_t, wscanf.. any functions with -W suffix for Windows API, and w- for CRT functions.
    wchar_t buffer[1024];
    DWORD readCount = 0;

    if (!ReadConsoleW(hInput, buffer, 1024, &readCount, NULL))
    {
        cerr << "Error: ReadConsoleW() failed\n";
        return;
    }
    buffer[readCount - 2] = L'\0';

    WriteConsoleW(hOutput, L"당신의 입력: ", 8, NULL, NULL);
    WriteConsoleW(hOutput, buffer, readCount - 2, NULL, NULL);
    WriteConsoleW(hOutput, L"\n\n", 2, NULL, NULL);
}

void TestUtf8ConsoleIo(HANDLE hInput, HANDLE hOutput)
{
    WriteConsoleA(hOutput, "\x1b[31mThe new UTF-8 way with Windows API!\x1b[0m\n", 45, NULL, NULL);
    WriteConsoleA(hOutput, "아무 문자나 입력해 주세요: ", 38, NULL, NULL);

    // With recent UTF-8 support 🎶
    // With classic char and -A variants
    // Make sure that WinAPI still internally uses UTF-16.
    // When we invoke -A functions, they convert characters to UTF-16 and go through the same process.
    // Only 1~2 percent overhead.
    // Source: http://utf8everywhere.org/
    char buffer[1024];
    DWORD readCount = 0;
    if (!ReadConsoleA(hInput, buffer, 1024, &readCount, NULL))
    {
        cerr << "Error: ReadConsoleA() failed\n";
        return;
    }
    buffer[readCount - 2] = '\0';

    WriteConsoleA(hOutput, "당신의 입력: ", 18, NULL, NULL);
    WriteConsoleA(hOutput, buffer, readCount - 2, NULL, NULL);
    WriteConsoleA(hOutput, "\n\n", 2, NULL, NULL);
}

void FixUnicodeNormalization()
{
    const char* filepath = "C:\\Users\\Capella87\\Dev\\fuckapple";
    wchar_t arr[257];
    if (!MultiByteToWideChar(CP_UTF8, 0, filepath, -1, arr, 0))
    {
        DWORD error_code = GetLastError();
        switch (error_code)
        {
        case ERROR_INSUFFICIENT_BUFFER:
            break;
        }
    }


}

void TestUTF8IoCrt()
{
    // Classic C style: Works well!
    char* arr = new char[256];

    printf("\x1b[32m이것은 CRT의 scanf_s를 사용했습니다.\n\x1b[0m");
    printf("아무 문자나 입력해 주세요: ");
    scanf_s("%s", arr, 256);
    printf("당신의 입력: %s\n\n", arr);

    delete[] arr;
}

void TestUTF8IoCpp()
{
    string arr;
    cout << "\x1b[32m이것은 그냥 C++ 표준 string을 사용했습니다.\n\x1b[0m";
    cout << "아무 문자나 입력해 주세요: ";
    cin >> arr;
    cout << "당신의 입력: " << arr << "\n\n";
}
