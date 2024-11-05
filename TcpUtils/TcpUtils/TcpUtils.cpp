// TcpUtils.cpp: define o ponto de entrada para o aplicativo.
//

#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "TCPClient.cpp"
using namespace std;

int main()
{
	TCPServer tcpServer = TCPServer(5060);
	while (true) 
	{ 
		char* recivedMessage = tcpServer.ThreatMessage();
		cout << recivedMessage;
	}
	//TCPClient* client = new TCPClient();
}
