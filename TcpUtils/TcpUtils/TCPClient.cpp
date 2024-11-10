#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024

#elif defined(__linux__)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#include <functional>
#include "Logger.h"

using namespace std;

class TCPClient
{
private:
    char* _address;
#if defined(_WIN32) || defined(_WIN64)
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };
    int _port = 5060;
#else
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };
    int _port = 5060;
#endif
    function<void(char[1024])> _event;

public:
    TCPClient(char* address, const int port, const function<void(char[1024])>& event)
    {
        _address = (char*)address;
        _event = event;
#if defined(_WIN32) || defined(_WIN64)
        Logger::LogInfo("Tcp server running in windows 64 mode!");
        _address = (char*)address;

        _port = port;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            Logger::LogDanger("Failed to initialize Winsock.");
            exit(EXIT_FAILURE);
        }

        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            Logger::LogDanger("Failed to create socket.");
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, address, &server_addr.sin_addr);

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            Logger::LogDanger("Connection failed.");
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
#else
        Logger::LogInfo("Tcp server running in linux mode!");
        _address = address;
        _port = (uint16_t)port;

        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            Logger::LogDanger("Failed to create socket.");
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, address, &server_addr.sin_addr) <= 0) {
            Logger::LogDanger("Invalid address/Address not supported.");
            exit(EXIT_FAILURE);
        }

        // Connect to server
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            Logger::LogDanger("Connection failed.");
            close(sock);
            exit(EXIT_FAILURE);
        }
#endif

    }

    void MessageReceive()
    {
        auto valread = recv(sock, buffer, BUFFER_SIZE, 0);
        _event(buffer);

    }

    void SendMessages(const char* message)
    {
        int sender;
        int tries = 0;
        do {
            sender = send(sock, message, strlen(message), 0);
            if (sender > 0)
                Logger::LogInfo("Message sent to server.");
            else
                Logger::LogDanger("Errpr to send to server.");
            tries++;
        } while (sender == -1 && tries < 5);
    }

    ~TCPClient()
    {
#if defined(_WIN32) || defined(_WIN64)
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
    }
};


