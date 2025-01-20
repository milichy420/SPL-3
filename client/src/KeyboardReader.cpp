#include "../include/KeyboardReader.h"
#include "../include/StompFrame.h"
#include <iostream>

KeyboardReader::KeyboardReader(ConnectionHandler &connectionHandler) : connectionHandler_(connectionHandler) {}

void KeyboardReader::run()
{
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        // Example: handle "login <host:port> <username> <password>" command
        if (line.substr(0, 5) == "login")
        {
            std::istringstream iss(line);
            std::string command, hostPort, username, password;
            iss >> command >> hostPort >> username >> password;

            StompFrame frame("CONNECT");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("host", hostPort);
            frame.addHeader("login", username);
            frame.addHeader("passcode", password);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendLine(frameStr))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
        }
        else if (line.substr(0, 4) == "join")
        {
            std::istringstream iss(line);
            std::string channel_name;
            iss >> channel_name;

            StompFrame frame("SUBSCRIBE");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("channel_name", channel_name);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendLine(frameStr))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
        }
        else if (line.substr(0, 4) == "exit")
        {
            std::istringstream iss(line);
            std::string channel_name;
            iss >> channel_name;

            StompFrame frame("UNSUBSCRIBE");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("channel_name", channel_name);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendLine(frameStr))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
        }
        else if (line.substr(0, 6) == "report")
        {
            std::istringstream iss(line);
            std::string file;
            iss >> file;

            StompFrame frame("SEND");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("file", file);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendLine(frameStr))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
        }
        else if (line.substr(0, 7) == "summary")
        {
            std::istringstream iss(line);
            std::string channel_name, user, file;
            iss >> channel_name >> user >> file;

            StompFrame frame("SUMMARY");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("channel_name", channel_name);
            frame.addHeader("user", user);
            frame.addHeader("file", file);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendLine(frameStr))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
        }
        else if (line.substr(0, 6) == "logout")
        {
            std::istringstream iss(line);
            std::string reciept;
            iss >> reciept;

            StompFrame frame("DICONNECT");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("reciept", reciept);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendLine(frameStr))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }
        }
    }
}