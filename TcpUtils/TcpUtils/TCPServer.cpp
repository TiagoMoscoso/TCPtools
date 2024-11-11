#include <iostream>
#include <cstring>
#include <cerrno>
#if defined(_WIN32) || defined(_WIN64)
# include <winsock2.h>
# include <ws2tcpip.h>
# pragma comment(lib, "ws2_32.lib") 
#else
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h> 
# include <unistd.h>
#endif
#include <list>
#include <queue>
#include <algorithm>
#include <functional>
#include "Logger.h"

using namespace std;
#define BUFFER_SIZE 1024

class TCPServer {
private:
    int _server_fd;
    struct sockaddr_in _address;
    int _opt = 1;
    int _addrlen = sizeof(_address);
    int _port = 5060;
    struct timeval _timeout;
    fd_set _readfds;
    list<int> _client_sockets;
    function<void(int, char[1024])> _event;

public:
    
    TCPServer(int port, const function<void(int, char[1024])>& event) 
    {
        _port = port;
        _event = event;

        #if defined(_WIN32) || defined(_WIN64)
        Logger::LogInfo("Tcp server running in windows 64 mode!");
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            Logger::LogDanger("Failed to initialize Winsock. Error Code: " + to_string(WSAGetLastError()) + "\n");
            exit(EXIT_FAILURE);
        }
        _server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_server_fd == INVALID_SOCKET) {
            Logger::LogDanger("Socket creation failed. Error Code: " + to_string(WSAGetLastError()) + "\n");
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&_opt, sizeof(_opt));
        #else
        Logger::LogInfo("Tcp server running in linux mode!");
        _server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (_server_fd == -1) {
            Logger::LogDanger("Socket creation failed. Error Code: " + to_string(errno) + "\n");
            exit(EXIT_FAILURE);
        }
        setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt));
        #endif

        memset(&_address, 0, sizeof(_address));
        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(_port);

        #if defined(_WIN32) || defined(_WIN64)
        if (::bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
            Logger::LogDanger("Bind failed: " + to_string(WSAGetLastError()) + "\n");
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        #else
        if (bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
            Logger::LogDanger("Bind failed: " + to_string(errno) + "\n");
            exit(EXIT_FAILURE);
        }
        #endif

        if (listen(_server_fd, 3) < 0) {
            Logger::LogDanger("Listen failed\n");
            exit(EXIT_FAILURE);
        }


        _timeout.tv_usec = 100000;

        Logger::LogDebug("Server is running on port " + to_string(port));
    }

    void Start() {
        int new_socket = 0;
        char buffer[1024] = { 0 };



        while (true) 
        {
            FD_ZERO(&_readfds);
            FD_SET(_server_fd, &_readfds);

            for (auto socket : _client_sockets) FD_SET(socket, &_readfds);
            
            if (select(_server_fd + 1, &_readfds, NULL, NULL, &_timeout) <= 0 && _client_sockets.size() == 0)
                continue;

            if (FD_ISSET(_server_fd, &_readfds)) {
                new_socket = accept(_server_fd, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);
                if (new_socket < 0) continue;
                FD_SET(new_socket, &_readfds);
                _client_sockets.push_front(new_socket);
            }

            auto disconnected_sockets = list<int>();

            if (!_client_sockets.empty())
                for (auto socket : _client_sockets)
                {
#if defined(_WIN32) || defined(_WIN64)
                    if (FD_ISSET(socket, &_readfds)) {
                        auto valread = recv(socket, buffer, sizeof(buffer), 0);
                        if (valread <= 0) {
                            Logger::LogInfo("Client Disconnected " + to_string(socket));
                            closesocket(socket);
                            FD_CLR(socket, &_readfds);
                            disconnected_sockets.push_front(socket);

                        }
                        else
                        {
                            _event(socket, buffer);
                        }
                    }

#else
                    auto valread = read(socket, buffer, sizeof(buffer));
                    if (valread < 0) {
                        Logger::LogInfo("Client Disconnected " + to_string(socket));

                        close(socket);
                        disconnected_sockets.push_front(socket);

                    }
                    else if(valread-1 != 0)
                    {
                        _event(socket, buffer);
                    }
#endif

                    for (int i = 0; i < 1024; ++i) buffer[i] = '\0';

                }

            if(!disconnected_sockets.empty())
                for (auto socket : disconnected_sockets) _client_sockets.remove(socket);
        }
    }
    

    bool SendMessages(int socket, char* message)
    {
        if (socket <= 0)return;
        try 
        {
            auto sender = send(socket, message, strlen(message), 0);

            if (bytes_sent < 0) 
            {
                if (errno == EPIPE) {
                    Logger::LogDanger("Erro de 'broken pipe': o socket foi fechado pelo outro lado.";
                }
                else {
                    Logger::LogDanger("Erro ao enviar mensagem: " + strerror(errno));
                }
                return false;
            }

            return true;
        }
        catch (exception& e) {
            Logger::LogDanger("SendMessage error: " + string(e.what()));
            return false;
        }
    }

    ~TCPServer() {
#if defined(_WIN32) || defined(_WIN64)
        closesocket(_server_fd);
        WSACleanup();
#else
        close(_server_fd);
#endif
    }
};
