// TcpUtils.cpp: define o ponto de entrada para o aplicativo.
//

#include "TcpUtils.h"
#include "TCPServer.cpp"
#include "TCPClient.cpp"
using namespace std;

int main()
{
	auto tcpServer = TCPServer(5060);
	tcpServer.Start();
	//TCPClient* client = new TCPClient("127.0.0.1", 5060);
}
