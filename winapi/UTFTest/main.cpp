#include <Windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// disable warning
#pragma warning(disable:4996)

enum BASEENUM
{
    HEX,
    BIN,
    DEC,
    OCTAL
};

char* get_binary(unsigned char* ch, char* result)
{
    char* rt = result;
    
    if (result == NULL)
        rt = (char*)malloc(sizeof(char) * 9);

    for (int i = 0; i < 8; i++)
    {
        rt[i] = '0';
    }
    rt[8] = '\0';

    unsigned char num = *ch;
    char* cursor = &rt[7];

    while (num && cursor >= rt)
    {
        *cursor = (num % 2) + '0';
        cursor--;
        num /= 2;
    }

    return rt;
}

int show_utf8_code(const char* ch, size_t len, BASEENUM MODE)
{
    unsigned char chr = 0;
    char* code = NULL;
    char* pt = NULL;
 
    switch (MODE)
    {
    case HEX:
        printf("Hex: 0x");
        for (size_t i = 0; i < len; i++)
        {
            chr = (unsigned char)ch[i];
            printf("%X ", chr);
        }
        break;

    case BIN:
        code = (char*)malloc(sizeof(char) * 9);
        printf("Binary: 0b");
        for (size_t i = 0; i < len; i++)
        {
            chr = (unsigned char)ch[i];
            get_binary(&chr, code);
            printf("%s ", code);
        }

        free(code);
        break;

    default:
        return -1;
    }
    putchar('\n');

    return 0;
}

void show_unicode_information(const char* target, size_t len)
{
    printf("UTF-8 Information\n");
    show_utf8_code(target, len, BIN);
    show_utf8_code(target, len, HEX);
}

int main(int argc, char** argv)
{
    const char* unicode_char = "😶‍🌫️";
    char input[101] = "";
    printf("%s\n", unicode_char);
    printf("Length: %llu\n", strlen(unicode_char));
    show_unicode_information(unicode_char, strlen(unicode_char));

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
