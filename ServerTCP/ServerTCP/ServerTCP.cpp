#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "Logger.h"
#include <functional>
#include "MessageContent.cpp"
#include <random>
#include <map>
using namespace std;

class Program {
private:
    int serverPort;
    TCPServer tcpServer;
    map<int, int> clientSocket;
    // ID --> SOCKET
public:
    Program(int port) :
        tcpServer(port, bind(&Program::MessageEvent, this, placeholders::_1, placeholders::_2)),
        serverPort(port){}

    int generateID() 
    {
        int random_number;
        do
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, 999);
            random_number = dis(gen);
        } while (clientSocket.find(random_number) != clientSocket.end());

        return random_number;
    }

    void MessageEvent(int socket, char buffer[1024]) 
    {
        try
        {
            auto messageContent = MessageContent(buffer);
            Logger::LogInfo("Data received from client " + to_string(socket) + " : " + messageContent.messagestr);


            switch (messageContent.type)
            {
            case 0:
                //start connection
                messageContent = MessageContent(0, generateID(), 0, 1024, "Connection successfully!");
                break;
            case 1:
                //end connection
                clientSocket.erase(messageContent.orig_uid);
                messageContent = MessageContent(1, messageContent.orig_uid, 0, 1024, "End Connection successfully!");
                break;
            case 2:
                //send serialize(ostringstream& out, MessageContent msgContent)
                try
                {
                    ostringstream oss;
                    messageContent.serialize(oss, messageContent);

                    auto binary_data = oss.str();
                    auto binary_cstr = (char*)binary_data.c_str();

                    tcpServer.SendMessages(clientSocket[messageContent.dest_uid], binary_cstr);

                    messageContent = MessageContent(2, messageContent.orig_uid, messageContent.dest_uid, 1024, ("Message sent to socket :" + to_string(clientSocket[messageContent.dest_uid])).c_str());
                }
                catch (exception ex)
                {
                    messageContent = MessageContent(2, messageContent.orig_uid, messageContent.dest_uid, 1024, ex.what());
                }
                break;
            case 3:
                string returnList;
                for (const auto& pair : clientSocket) returnList += to_string(pair.second) + ";";
                messageContent = MessageContent(3, messageContent.orig_uid, messageContent.dest_uid, 1024, returnList.c_str());
                break;
            }
            ostringstream oss;
            messageContent.serialize(oss, messageContent);

            auto binary_data = oss.str();
            auto binary_cstr = (char*)binary_data.c_str();

            tcpServer.SendMessages(socket, binary_cstr);

            Logger::LogInfo("Response sent to the client.");
        }
        catch (exception ex) { Logger::LogDanger(ex.what()); }
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
