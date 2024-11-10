#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "Logger.h"
#include "TCPClient.cpp"
#include "MessageContent.cpp"
#include <functional>

using namespace std;

class Program {
private:
    int serverPort;
    TCPClient tcpClient;
    short id = -1;

public:
    Program(int port) :
        tcpClient((char*)"127.0.0.1",port, bind(&Program::MessageEvent, this, placeholders::_1)),
        serverPort(port) {}

    void MessageEvent(char* buffer)
    {
        auto messageContent = MessageContent(buffer);
        Logger::LogInfo("Data received from server : " + messageContent.messagestr);
        

        switch (messageContent.type)
        {
            case 0:
                id = messageContent.orig_uid;
                Logger::LogDebug("Connection accepted my ID is : " + to_string(id));
                break;
        }
    }

    void ClientSendMessage(MessageContent& msgContent)
    {
        ostringstream oss;

        msgContent.serialize(oss, msgContent);

        auto binary_data = oss.str();
        const char* binary_cstr = binary_data.c_str();

        tcpClient.SendMessages(binary_cstr);
    }

    void Run() 
    {

        while (true)
        {
            auto messageContent = MessageContent(0, 0, 0, 1024, "Connection Request!");
            ClientSendMessage(messageContent);
            tcpClient.MessageReceive();
        }
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


