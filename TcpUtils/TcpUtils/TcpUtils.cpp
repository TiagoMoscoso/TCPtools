// TcpUtils.cpp: define o ponto de entrada para o aplicativo.
//

#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "TCPClient.cpp"
using namespace std;

void MessageEvent(int socket, char buffer[1024])
{

	cout << "Data recived from client " << socket << " :" << buffer << endl;

	auto response = "server ok";

	//tcpServer.SendMessage(socket, (char*)response);

	cout << "Response sent to the client." << endl;
}

int main()
{
	auto tcpServer = TCPServer(5060, MessageEvent);
	tcpServer.Start();
	//TCPClient* client = new TCPClient("127.0.0.1", 5060);
}