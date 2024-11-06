#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib
#define BUFFER_SIZE 1024

class TCPClient
{
private:
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };
    const char* message = "Hello from client!";
    char _address[12] = "127.0.0.1";
    u_short _port = 5060;
public:
    TCPClient(const char address[12], u_short port)
    {
        strcpy(_address, address);
        _port = port;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Failed to create socket." << std::endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); 

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failed." << std::endl;
            closesocket(sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        SendMessages();
        MessageRecive();

    }

    void MessageRecive()
    {
        int valread = recv(sock, buffer, BUFFER_SIZE, 0);
        buffer[valread] = '\0';
        std::cout << "Message from server: " << buffer << std::endl;

    }

    void SendMessages()
    {
        send(sock, message, strlen(message), 0);
        std::cout << "Message sent to server." << std::endl;
    }

    ~TCPClient()
    {
        closesocket(sock);
        WSACleanup();
    }
};