#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std; // Added namespace

#define PORT 12346 // Use a different port
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    SOCKET server_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket (SOCK_DGRAM for UDP)
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // 2. Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    cout << "UDP Server listening on port " << PORT << "..." << endl;

    // 3. Receive data (no accept needed)
    // recvfrom will block until a packet arrives
    int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (bytes_received == SOCKET_ERROR) {
        cerr << "recvfrom failed: " << WSAGetLastError() << endl;
    } else {
        cout << "Received from client: " << buffer << endl;

        // 4. Send a reply back to the client
        const char *hello = "Hello from UDP Server!";
        sendto(server_socket, hello, strlen(hello), 0, (struct sockaddr *)&client_addr, client_addr_len);
        cout << "Hello message sent." << endl;
    }

    // 5. Close socket
    closesocket(server_socket);
    WSACleanup();

    return 0;
}