#include <iostream>
#include <thread>
#include "../include/ConnectionHandler.h"
#include "../include/KeyboardReader.h"
#include "../include/SocketReader.h"

int main(int argc, char *argv[])
{
	KeyboardReader keyboardReader;

	std::thread keyboardThread(&KeyboardReader::run, &keyboardReader);

	keyboardThread.join();

	return 0;
}