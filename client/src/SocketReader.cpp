#include "../include/SocketReader.h"
#include "../include/StompFrame.h"
#include <iostream>

SocketReader::SocketReader(ConnectionHandler &connectionHandler, KeyboardReader &keyboardReader) : connectionHandler_(connectionHandler), keyboardReader_(keyboardReader) {}

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

        StompFrame frame = StompFrame::fromString(answer);
        if (frame.getCommand() == "CONNECTED")
        {
            std::cout << "Login successful." << std::endl;
        }
        else if (frame.getCommand() == "RECEIPT")
        {
            std::string receiptId = frame.getHeader("receipt-id");
            std::cout << "receipt-id: " << receiptId << std::endl;
        }
        else if (frame.getCommand() == "MESSAGE")
        {
            std::cout << "Received message: " << frame.getBody() << std::endl;
        }
        else if (frame.getCommand() == "ERROR")
        {
            std::cout << "Error: " << frame.getBody() << std::endl;
        }
        else
        {
            std::cout << "Received frame: " << frame.toString() << std::endl;
        }
    }
}