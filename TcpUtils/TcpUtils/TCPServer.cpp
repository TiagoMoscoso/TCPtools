#include <iostream>
#include <cstring>
#include <cerrno>
#ifdef _WIN64
# include <winsock2.h>
# include <ws2tcpip.h>
# pragma comment(lib, "ws2_32.lib") 
#else
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h> 
# include <unistd.h>
#endif
using namespace std;
#define BUFFER_SIZE 1024
class TCPServer {
private:
    int _server_fd;
    struct sockaddr_in _address;
    int _opt = 1;
    int _addrlen = sizeof(_address);
    int _port = 5060;
public:
    TCPServer(int port) {
        _port = port;
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        _server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_server_fd == INVALID_SOCKET) {
            printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
            WSACleanup();
            exit(EXIT_FAILURE);
        }


        #ifdef _WIN64
        setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&_opt, sizeof(_opt));
        #else
        setsockopt(_server_fd, SOL_SOCKET, SO_REUSEPORT, &_opt, sizeof(_opt));
        #endif
        memset(&_address, 0, sizeof(_address));
        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(_port);

        if (bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
            fprintf(stderr, "Bind failed: %d\n", WSAGetLastError());
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        if (listen(_server_fd, 3) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }

        printf("SERVER RUNNING\n");
    }

    char* ThreatMessage() {
        int new_socket;
        char buffer[140] = { 0 };

        new_socket = accept(_server_fd, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            return  { 0 };
        }
        #ifdef _WIN64
        recv(new_socket, buffer, sizeof(buffer), 0);
        #else
        read(new_socket, buffer, sizeof(buffer));
        #endif
        

        const char* response = "Ok";
        send(new_socket, response, strlen(response), 0);

        return buffer;
    }

    ~TCPServer() {

        #ifdef _WIN64
        closesocket(_server_fd);
        WSACleanup();
        #else
        close(new_socket);
        #endif
    }
};
