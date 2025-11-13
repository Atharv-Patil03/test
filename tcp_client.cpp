#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std; // Added namespace

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    SOCKET sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket (SOCK_STREAM for TCP)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation error: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    // 2. Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        cerr << "Connection Failed: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // 3. Communicate (send/recv)
    const char *hello = "Hello from TCP Client!";
    send(sock, hello, strlen(hello), 0);
    cout << "Hello message sent." << endl;

    recv(sock, buffer, BUFFER_SIZE, 0);
    cout << "Server replied: " << buffer << endl;

    // 4. Close socket
    closesocket(sock);
    WSACleanup();

    return 0;
}