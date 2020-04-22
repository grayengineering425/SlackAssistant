#include "Server/Server.h"

int main()
{
	Server server("127.0.0.1", "5000");

	server.listenSocket();

	return 0;
}