#include "../WinSocket/Common.hpp"

int main(int argc, char** argv)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa))
    {
        return -1;
    }

    u_short x1 = 0x1234;
    u_long y1 = 0x12345678;
    u_short x2;
    u_long y2;

    // Host Byte -> Network Byte
    printf("%#x -> %#x\n", x1, x2 = htons(x1));
    printf("%#x -> %#x\n", y1, y2 = htonl(y1));

    // Network Byte -> Host Byte
    printf("%#x -> %#x\n", x2, x1 = ntohs(x2));
    printf("%#x -> %#x\n", y2, y1 = ntohl(y2));

    // Inappropriate usage
    printf("\x1b[31m%#x -> %#x\x1b[0m\n", x1, htonl(x1));

    WSACleanup();
    return 0;
}
