#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "Logger.h"
#include "TCPClient.cpp"
#include "MessageContent.cpp"
#include <functional>
#include <iostream>
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

        switch (messageContent.type)
        {
            case 0:
                id = messageContent.orig_uid;
                Logger::LogInfo("Data received from server : " + messageContent.messagestr);
                Logger::LogDebug("Connection accepted my ID is : " + to_string(id));
                break;
            case 3:
                Logger::LogDebug("List of connected clients : " + messageContent.messagestr);
                break;
            default:
                Logger::LogInfo("Data received from server : " + messageContent.messagestr);
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
        auto messageContent = MessageContent(0, 0, 0, 1024, "OI");
        ClientSendMessage(messageContent);
        tcpClient.MessageReceive();
        while (true)
        {

            short int type = 3;
            short int destinataryId;
            char message[1024];

            memset(message, 0, sizeof(message));


            messageContent = MessageContent(type, id, 0, 16, "List_Connections");
            ClientSendMessage(messageContent);
            tcpClient.MessageReceive();

            type = 2;
            memset(message, 0, sizeof(message));

            Logger::LogInfo("Enter the destinatary ID or enter with -1 to stop the application: ");
            cin >> destinataryId;

            if (destinataryId == -1) break;

            Logger::LogInfo("Enter the message: ");

            cin >> message;
            

            messageContent = MessageContent(type, id, destinataryId, sizeof(message), message);
            ClientSendMessage(messageContent);
            tcpClient.MessageReceive();
            if (id == destinataryId) tcpClient.MessageReceive();
            
        }
        messageContent = MessageContent(1, id, 0, 1024, "TCHAU");
        ClientSendMessage(messageContent);
        tcpClient.MessageReceive();
    }
};

int main()
{
    Program program(5060);
    program.Run();

    return 0;
}


