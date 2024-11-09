#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "Logger.h"
#include <functional>

using namespace std;

class Program {
private:
    int serverPort;
    TCPServer tcpServer;


public:
    Program(int port) :
        tcpServer(port, bind(&Program::MessageEvent, this, std::placeholders::_1, std::placeholders::_2)),
        serverPort(port){}

    void MessageEvent(int socket, char buffer[1024]) {
        Logger::LogInfo("Data received from client " + to_string(socket) + " : " + buffer);

        auto response = "server ok";

        tcpServer.SendMessages(socket, (char*)response);

        Logger::LogInfo("Response sent to the client.");
    }

    void Run() {
        tcpServer.Start();

    }
};

int main() 
{
    Program program(5060);

    program.Run();

    return 0;
}
