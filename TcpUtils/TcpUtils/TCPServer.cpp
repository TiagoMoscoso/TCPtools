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
#include <list>
#include <queue>
#include <algorithm>

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
public:
    queue<char[1024]> MessageQueue;

    TCPServer(int port) 
    {
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

        _timeout.tv_sec = 1;
        _timeout.tv_usec = 0;

        cout << "Server is running in port " << port;
    }

    void Start()
    {

        int new_socket = 0;
        char buffer[1024] = { 0 };


        FD_ZERO(&_readfds);
        FD_SET(_server_fd, &_readfds);

        if (select(_server_fd + 1, &_readfds, NULL, NULL, &_timeout) <= 0 && _client_sockets.size() == 0)
            return;

        if (FD_ISSET(_server_fd, &_readfds))
            if ((new_socket = accept(_server_fd, (struct sockaddr*)&_address, &_addrlen)) < 0)
                    return;


        FD_SET(new_socket, &_readfds);


        auto it = find(_client_sockets.begin(), _client_sockets.end(), new_socket);
        if (it == _client_sockets.end()) _client_sockets.push_front(new_socket);

        auto disconected_sockets = list<int>();

        for (auto socket : _client_sockets)
        {
                
            if (FD_ISSET(socket, &_readfds))
            {
                auto valread = recv(socket, buffer, sizeof(buffer), 0);
                if (valread <= 0) 
                {
                    cout << "Client Disconected " << socket << endl;
                    closesocket(socket);
                    FD_CLR(socket, &_readfds);
                    disconected_sockets.push_front(socket);
                }
                else if (valread > 0)
                {
                    cout << "Data recived from client "<< socket << " :" << buffer << endl;

                    //MessageQueue.push(buffer);

                    auto response = "server ok";

                    SendMessage(socket, (char*)response);

                    cout << "Response sent to the client." << endl;
                }
            }
        }

        for (auto socket : disconected_sockets)
        {
            _client_sockets.remove(socket);
        }
        
    }

    bool SendMessage(int socket, char* message) 
    { 
        try
        {
            send(socket, message, strlen(message), 0);
            return true;
        }
        catch (exception)
        {
            return false;
        }
    }


    ~TCPServer() 
    {

        #ifdef _WIN64
        closesocket(_server_fd);
        WSACleanup();
        #else
        close(new_socket);
        #endif
    }
};
