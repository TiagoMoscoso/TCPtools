#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "Logger.h"
#include "TCPClient.cpp"
using namespace std;

class Program {
private:
    int serverPort;
    TCPClient tcpClient;


public:
    Program(int port) :
        tcpClient("127.0.0.1",port, MessageEvent),
        serverPort(port) {}

    static void MessageEvent(char* buffer)
    {
        Logger::LogInfo("Data received from server : " + string(buffer));
    }

    void Run() {
        tcpClient.SendMessages();
        tcpClient.MessageReceive();
    }
};

static void Event(char* buffer)
{
    Logger::LogInfo("Data received from server : " + string(buffer));
}

int main()
{
    Program program(5060);
    program.Run();

    return 0;
}


