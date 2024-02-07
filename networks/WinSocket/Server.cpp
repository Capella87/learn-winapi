#include "Common.hpp"
#include <locale.h>
#include <fcntl.h>
#include <io.h>

#define SERVERPORT 1989
#define BUFSIZE 512

DWORD WINAPI TCPServerIPv4(LPVOID arg)
{
    int retval;

    // Create a socket
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
        err_quitW(L"socket()");

    // Bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    retval = bind(listen_sock, (struct sockaddr*)&serveraddr,
        sizeof(serveraddr));

    if (retval == SOCKET_ERROR)
        err_quitW(L"bind()");

    // Listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
        err_quitW(L"listen()");

    // Variables to use on data communication
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    wchar_t wipv4addr[16];
    wchar_t wbuf[BUFSIZE + 1];
    DWORD ipv4addr_len = INET_ADDRSTRLEN;

    while (1)
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            err_displayW(L"accept()");
            break;
        }
        // Show connected client's information
        WSAAddressToStringW((struct sockaddr*)&clientaddr, sizeof(clientaddr), NULL, wipv4addr, &ipv4addr_len);
        wprintf(L"\n[TCP 서버] 클라이언트 접속⚡: IP Address: %ls, Port: %d\n",
            wipv4addr, ntohs(clientaddr.sin_port));

        // Data communication with client
        while (1)
        {
            // Receive data
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR)
            {
                err_displayW(L"recv()");
                break;
            }
            else if (!retval) break;

            // Print received data
            buf[retval] = '\0';

            int utf8_len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
            

            mbstowcs(wbuf, (const char*)buf, strlen(buf) + 1);
            wprintf(L"%ls", wbuf);
        }

        // Close socket
        closesocket(client_sock);
        wprintf(L"\n[TCP 서버] 클라이언트 종료🚧: IP Address: %ls, Port: %d\n",
            wipv4addr, ntohs(clientaddr.sin_port));
    }

    // Close socket
    closesocket(listen_sock);
    return 0;
}

// TCP Server (IPv6)
DWORD WINAPI TCPServerIPv6(LPVOID arg)
{
    int retval;

    // Create a socket
    SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quitW(L"socket()");

    // Disable dual stack (Default: off in Windows anyway)
    // It seems to use IPv6 only
    int no = 1;
    setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&no, sizeof(no));

    // bind()
    struct sockaddr_in6 serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(SERVERPORT);

    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quitW(L"bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quitW(L"listen()");

    // Variables for data communication
    SOCKET client_sock;
    struct sockaddr_in6 clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    wchar_t wipv6addr[INET6_ADDRSTRLEN];
    wchar_t wbuf[BUFSIZE + 1];
    DWORD ipv6addr_len = INET6_ADDRSTRLEN;

    while (1)
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            err_displayW(L"accept()");
            break;
        }

        // Show connected client's information
        WSAAddressToStringW((struct sockaddr*)&clientaddr, sizeof(clientaddr), NULL, wipv6addr, &ipv6addr_len);
        wprintf(L"\n[TCP 서버] 클라이언트 접속: IP Address: %ls, Port: %d\n",
            wipv6addr, ntohs(clientaddr.sin6_port));

        // Data communication with client
        while (1)
        {
            // Receive data
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR)
            {
                err_displayW(L"recv()");
                break;
            }
            else if (!retval) break;

            // Print received data
            buf[retval] = '\0';
            mbstowcs(wbuf, (const char*)buf, strlen(buf) + 1);
            wprintf(L"%ls", wbuf);
        }

        // Close socket
        closesocket(client_sock);
        wprintf(L"\n[TCP 서버] 클라이언트 종료: IP Address: %ls, Port: %d\n",
            wipv6addr, ntohs(clientaddr.sin6_port));
    }

    // Close socket
    closesocket(listen_sock);
    return 0;
}

int wmain(int argc, wchar_t* argv[])
{
    setlocale(LC_ALL, "");
    _setmode(_fileno(stdout), _O_U8TEXT);


    // Initialize WinSock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa))
    {
        return 1;
    }

    // Run both servers simultaneously by using multithreading
    HANDLE hThread[2];
    hThread[0] = CreateThread(NULL, 0, TCPServerIPv4, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, TCPServerIPv6, NULL, 0, NULL);
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

    // Terminate WinSock
    WSACleanup();

    return 0;
}
