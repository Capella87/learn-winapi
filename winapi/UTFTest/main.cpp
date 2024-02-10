#include <Windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// disable warning
#pragma warning(disable:4996)

typedef enum BASEENUM
{
    HEX,
    BIN,
    DEC,
    OCTAL
} baseenum;

const char* get_binary(unsigned char* ch)
{
    char rt[9] = "00000000";
    char num = ch[0];
    char* cursor = &rt[7];

    while (num && cursor > rt)
    {
        *cursor = (num % 2) + '0';
        cursor--;
        num /= 2;
    }

    return rt;
}

unsigned char get_utf8_code(const char* ch, size_t len, baseenum mode)
{
    unsigned char chr = 0;
    // Disassemble
    printf("0x");
    for (size_t i = 0; i < len; i++)
    {
        chr = (unsigned char)ch[i];
        printf("%x ", chr);
    }
    putchar('\n');

    return (unsigned char)0;
}

int main(int argc, char** argv)
{
    const char* latest_emoji = "🪫";
    printf("%s\n", latest_emoji);
    printf("Length: %llu\n", strlen(latest_emoji));
    get_utf8_code(latest_emoji, strlen(latest_emoji), HEX);

    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;

    // GetVersion() is legacy and not properly working unless defining version definition in manifest file explicitly.
    dwVersion = GetVersion();

    // Get the Windows version.

    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.

    if (dwVersion < 0x80000000)
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Version is %d.%d (%d)\n",
        dwMajorVersion,
        dwMinorVersion,
        dwBuild);

    // IsWindows* family functions are NOT legacy and recommended, but version definition is required to run properly.
    if (IsWindows10OrGreater())
    {
        printf("Yes! you're using Windows 10 or later\n");
    }

    return 0;
}
