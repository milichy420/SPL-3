#include "../include/SocketReader.h"
#include "../include/StompFrame.h"
#include <iostream>

// SocketReader::SocketReader(ConnectionHandler &connectionHandler, KeyboardReader &keyboardReader) : connectionHandler_(connectionHandler), keyboardReader_(keyboardReader) {}
SocketReader::SocketReader(KeyboardReader &keyboardReader) : keyboardReader_(keyboardReader) {}

void SocketReader::run()
{
    bool stopThread_ = false;
    while (!stopThread_)
    {
        std::string answer;
        if (!keyboardReader_.getConnectionHandler().getFrameAscii(answer, '\0'))
        {
            std::cout << "Disconnected. Exiting..." << std::endl;
            break;
        }

        StompFrame frame = StompFrame::fromString(answer);
        if (frame.getCommand() == "CONNECTED")
        {
            std::cout << "Login successful." << std::endl;
            keyboardReader_.setLoggedIn(true);
        }
        else if (frame.getCommand() == "RECEIPT")
        {
            std::string receiptId = frame.getHeader("receipt-id");
            StompFrame sentFrame = keyboardReader_.getFrame(receiptId);

            if (sentFrame.getCommand() == "DISCONNECT")
            {
                std::cout << "Disconnecting..." << std::endl;
                keyboardReader_.setLoggedIn(false);
                keyboardReader_.getConnectionHandler().close();
                stopThread_ = true;
                keyboardReader_.setStopThread(true);
                break;
            }
            else if (sentFrame.getCommand() == "SUBSCRIBE")
            {
                std::cout << "Joined channel " << sentFrame.getHeader("destination") << std::endl;
            }
            else if (sentFrame.getCommand() == "UNSUBSCRIBE")
            {
                std::cout << "Exited channel " << sentFrame.getHeader("id") << std::endl;
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
            std::cout << "Received message: \n"
                      << std::endl;
            std::cout << "Destination: " << destination << "\n"
                      << std::endl;
            std::cout << message << std::endl;

            keyboardReader_.addMessageToChannel(destination.substr(1), message);
        }
        else if (frame.getCommand() == "ERROR")
        {
            std::cout << "Error: " << frame.getBody() << std::endl;

            keyboardReader_.setLoggedIn(false);
            keyboardReader_.getConnectionHandler().close();
            stopThread_ = true;
            keyboardReader_.setStopThread(true);
        }
        else
        {
            std::cout << "Received frame: " << frame.toString() << std::endl;
        }
    }
}