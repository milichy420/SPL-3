#include "../include/SocketReader.h"
#include <iostream>

SocketReader::SocketReader(ConnectionHandler &connectionHandler) : connectionHandler_(connectionHandler) {}

void SocketReader::run()
{
    while (true)
    {
        std::string answer;
        if (!connectionHandler_.getLine(answer))
        {
            std::cout << "Disconnected. Exiting...\n"
                      << std::endl;
            break;
        }
        std::cout << "Reply: " << answer << std::endl;
    }
}