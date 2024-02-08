#include <Windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// disable warning
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
    const char* latest_emoji = "🛜";
    printf("%s\n", latest_emoji);
    printf("Length: %llu\n", strlen(latest_emoji));

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
