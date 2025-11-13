#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> // For CreateDirectory

#pragma comment(lib, "ws2_32.lib")

using namespace std; // Added namespace

#define PORT 12346 // Use a different port
#define BUFFER_SIZE 1024

int main() {
    // Create a directory for received files
    CreateDirectoryA("received_files_cpp", NULL);

    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl; return 1;
    }

    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 2. Setup socket, bind, and listen
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket failed: " << WSAGetLastError() << endl; WSACleanup(); return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl; closesocket(server_fd); WSACleanup(); return 1;
    }
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl; closesocket(server_fd); WSACleanup(); return 1;
    }

    cout << "File server is listening on port " << PORT << "..." << endl;

    // 3. Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == INVALID_SOCKET) {
        cerr << "Accept failed: " << WSAGetLastError() << endl; closesocket(server_fd); WSACleanup(); return 1;
    }

    // 4. First, receive the filename
    memset(buffer, 0, BUFFER_SIZE);
    recv(new_socket, buffer, BUFFER_SIZE, 0);
    string filename = "received_files_cpp\\"; // Use Windows path separator
    filename += buffer;
    cout << "Receiving file: " << filename << endl;

    // 5. Create a file stream to write the received data
    ofstream received_file(filename, ios::binary);
    if (!received_file) {
        cerr << "Failed to open file for writing." << endl;
    } else {
        // 6. Receive the file content
        int bytes_received;
        while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            received_file.write(buffer, bytes_received);
        }
        cout << "File received successfully." << endl;
        received_file.close();
    }

    // 7. Close everything
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}