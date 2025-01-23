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
            keyboardReader_.setLoggedIn(true);
        }
        else if (frame.getCommand() == "RECEIPT")
        {
            std::string receiptId = frame.getHeader("receipt-id");
            std::cout << "Receipt received for frame: " << receiptId << std::endl;
            StompFrame sentFrame = keyboardReader_.getFrame(receiptId);
            std::cout << "Associated frame: " << sentFrame.toString() << std::endl;

            if (sentFrame.getCommand() == "DISCONNECT")
            {
                std::cout << "Disconnecting..." << std::endl;
                keyboardReader_.setLoggedIn(false);
                connectionHandler_.close();
                break;
            }
            else if (sentFrame.getCommand() == "SUBSCRIBE")
            {
                std::cout << "Joind channel " << sentFrame.getHeader("destination") << std::endl;
                break;
            }
            else if (sentFrame.getCommand() == "UNSUBSCRIBE")
            {
                std::cout << "Exited channel " << sentFrame.getHeader("destination") << std::endl;
                break;
            }
            else if (sentFrame.getCommand() == "SEND")
            {
                /* code */
            }
        }
        else if (frame.getCommand() == "MESSAGE")
        {
            std::string destination = frame.getHeader("destination");
            std::string message = frame.getBody();
            std::cout << "Received message: /n" << std::endl;
            std::cout << "Destination: " << destination << "/n" << std::endl;
            std::cout << "Received message: " << message << std::endl;

            keyboardReader_.addMessageToChannel(destination, message);
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