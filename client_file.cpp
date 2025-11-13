#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> // For Sleep()

#pragma comment(lib, "ws2_32.lib")

using namespace std; // Added namespace

#define PORT 12346 // Match the server's port
#define BUFFER_SIZE 1024

// Helper function to get the base filename from a Windows path
string getBasename(const string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl; return 1;
    }

    SOCKET sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 2. Setup socket and connect
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation error: " << WSAGetLastError() << endl; WSACleanup(); return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        cerr << "Connection Failed: " << WSAGetLastError() << endl; closesocket(sock); WSACleanup(); return -1;
    }

    // 3. Get file path from user
    string filepath;
    cout << "Enter the path of the file to send: ";
    cin >> filepath;

    // 4. Open the file
    ifstream file_to_send(filepath, ios::binary);
    if (!file_to_send) {
        cerr << "Error: File not found or could not be opened." << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 5. Extract filename and send it first
    string filename = getBasename(filepath);
    send(sock, filename.c_str(), filename.length(), 0);
    cout << "Sending file: " << filename << endl;

    // A small delay to ensure server processes filename
    Sleep(500); // 500 milliseconds

    // 6. Read file and send its contents
    while (!file_to_send.eof()) {
        file_to_send.read(buffer, BUFFER_SIZE);
        if (file_to_send.gcount() > 0) {
            if (send(sock, buffer, file_to_send.gcount(), 0) == SOCKET_ERROR) {
                cerr << "Send failed: " << WSAGetLastError() << endl;
                break;
            }
        }
    }

    cout << "File sent successfully." << endl;

    // 7. Close everything
    file_to_send.close();
    closesocket(sock);
    WSACleanup();

    return 0;
}