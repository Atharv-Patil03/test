#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std; // Added namespace

#define PORT 12346 // Match the UDP server's port
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    SOCKET client_socket;
    struct sockaddr_in server_addr;
    int server_addr_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket (SOCK_DGRAM for UDP)
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation error: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    // 2. Set up the server's address (no connect)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Send the message
    const char *hello = "Hello from UDP Client!";
    if (sendto(client_socket, hello, strlen(hello), 0, (struct sockaddr *)&server_addr, server_addr_len) == SOCKET_ERROR) {
        cerr << "sendto failed: " << WSAGetLastError() << endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    cout << "Hello message sent." << endl;

    // 4. Receive the reply
    if (recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &server_addr_len) == SOCKET_ERROR) {
        cerr << "recvfrom failed: " << WSAGetLastError() << endl;
    } else {
        cout << "Server replied: " << buffer << endl;
    }

    // 5. Close socket
    closesocket(client_socket);
    WSACleanup();

    return 0;
}