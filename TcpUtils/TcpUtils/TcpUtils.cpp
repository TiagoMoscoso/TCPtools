// TcpUtils.cpp: define o ponto de entrada para o aplicativo.
//

#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "TCPClient.cpp"
#include "Logger.h"
using namespace std;

void MessageEvent(int socket, char buffer[1024])
{
	Logger::LogInfo("Data received from client " + to_string(socket) + " : " + buffer);

	auto response = "server ok";

	//tcpServer.SendMessage(socket, (char*)response);

	Logger::LogInfo("Response sent to the client.");
}

int main()
{
	auto tcpServer = TCPServer(5060, MessageEvent);
	tcpServer.Start();
	//TCPClient* client = new TCPClient("127.0.0.1", 5060);
}