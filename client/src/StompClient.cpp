#include <iostream>
#include <thread>
#include "../include/ConnectionHandler.h"
#include "../include/KeyboardReader.h"
#include "../include/SocketReader.h"

int main(int argc, char *argv[])
{
	const std::string HOST = "stomp.cs.bgu.ac.il";
	const short PORT = 7777;

	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " host port" << std::endl
				  << std::endl;
		return -1;
	}
	std::string host = argv[1];
	short port = atoi(argv[2]);

	ConnectionHandler connectionHandler(host, port);
	if (!connectionHandler.connect())
	{
		std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
		return 1;
	}

	KeyboardReader keyboardReader(connectionHandler);
	SocketReader socketReader(connectionHandler);

	std::thread keyboardThread(&KeyboardReader::run, &keyboardReader);
	std::thread socketThread(&SocketReader::run, &socketReader);

	keyboardThread.join();
	socketThread.join();

	return 0;
}