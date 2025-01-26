#include "../include/KeyboardReader.h"
#include "../include/StompFrame.h"
#include "../include/event.h"
#include "../include/SocketReader.h"
#include "../include/ConnectionHandler.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>

KeyboardReader::KeyboardReader() : connectionHandler_("127.0.0.1", 7777), sentFrames_(), channelMessages_(),
                                   topicToSubscriptionId_(), subscriptionIdCounter_(0), loggedIn_(false), user_(""),
                                   socketThread_(), stopThread_(false) {}

KeyboardReader::~KeyboardReader()
{
    stopSocketThread();
}

void KeyboardReader::run()
{
    const std::string HOST = "stomp.cs.bgu.ac.il";
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);

        if (stopThread_)
        {
            stopSocketThread();
        }

        if (line.substr(0, 5) == "login")
        {
            std::istringstream iss(line);
            std::string command, hostPort, username, password;
            iss >> command >> hostPort >> username >> password;

            std::string host = hostPort.substr(0, hostPort.find(':'));
            short port = std::stoi(hostPort.substr(hostPort.find(':') + 1));

            connectionHandler_.setHost(host);
            connectionHandler_.setPort(port);
            if (!connectionHandler_.connect())
            {
                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                continue;
            }

            StompFrame frame("CONNECT");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("host", HOST);
            frame.addHeader("login", username);
            frame.addHeader("passcode", password);

            user_ = username;

            std::string frameStr = frame.toString();

            if (!connectionHandler_.sendFrameAscii(frameStr, '\0'))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }

            // startSocketReader();
            stopThread_ = false;
            SocketReader socketReader(*this);
            socketThread_ = std::thread(&SocketReader::run, socketReader);
            // setLoggedIn(true);
        }
        else if (line.substr(0, 4) == "join")
        {
            if (!loggedIn_)
            {
                std::cout << "You must be logged in to join a channel." << std::endl;
                continue;
            }

            std::istringstream iss(line);
            std::string command, channel_name;
            iss >> command >> channel_name;

            int subscriptionId = KeyboardReader::generateSubscriptionId();

            topicToSubscriptionId_[channel_name] = subscriptionId;

            StompFrame frame("SUBSCRIBE");
            frame.addHeader("destination", channel_name);
            frame.addHeader("id", std::to_string(subscriptionId));
            std::string receiptId = connectionHandler_.generateReceiptId();
            frame.addHeader("receipt", receiptId);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendFrameAscii(frameStr, '\0'))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }

            addFrame(receiptId, frame);
            channelMessages_[channel_name] = std::vector<std::string>(); // Initialize an empty vector for the channel
        }
        else if (line.substr(0, 4) == "exit")
        {
            if (!loggedIn_)
            {
                std::cout << "You must be logged in to join a channel." << std::endl;
                continue;
            }

            std::istringstream iss(line);
            std::string command, channel_name;
            iss >> command >> channel_name;

            StompFrame frame("UNSUBSCRIBE");

            std::string receiptId = connectionHandler_.generateReceiptId();
            int subscriptionId = KeyboardReader::topicToSubscriptionId_[channel_name];

            frame.addHeader("id", std::to_string(subscriptionId));
            frame.addHeader("receipt", receiptId);

            std::string frameStr = frame.toString();
            std::cout << "Sending frame:\n"
                      << frameStr << std::endl;
            if (!connectionHandler_.sendFrameAscii(frameStr, '\0'))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }

            addFrame(receiptId, frame);
        }
        else if (line.substr(0, 6) == "report")
        {
            if (!loggedIn_)
            {
                std::cout << "You must be logged in to join a channel." << std::endl;
                continue;
            }

            std::istringstream iss(line);
            std::string command, file;
            iss >> command >> file;

            names_and_events reportEvents = parseEventsFile(file);
            if (reportEvents.channel_name.empty())
            {
                std::cout << "Error parsing file: " << file << std::endl;
                continue;
            }
            if (topicToSubscriptionId_.find(reportEvents.channel_name) == topicToSubscriptionId_.end())
            {
                std::cout << "Join the channel " << reportEvents.channel_name << " before reporting." << std::endl;
                continue;
            }
            for (auto &event : reportEvents.events)
            {
                event.setEventOwnerUser(user_);
                StompFrame eventFrame("SEND");
                eventFrame.addHeader("destination", "/" + reportEvents.channel_name);
                std::string receiptId = connectionHandler_.generateReceiptId();

                eventFrame.addHeader("receipt", receiptId);

                eventFrame.setBody(event.toString());

                std::string eventFrameStr = eventFrame.toString();
                if (!connectionHandler_.sendFrameAscii(eventFrameStr, '\0'))
                {
                    std::cout << "Disconnected. Exiting...\n"
                              << std::endl;
                    break;
                }
                addFrame(receiptId, eventFrame);
                // addMessageToChannel(reportEvents.channel_name, event.toString());
            }
        }
        else if (line.substr(0, 7) == "summary")
        {
            if (!loggedIn_)
            {
                std::cout << "You must be logged in to join a channel." << std::endl;
                continue;
            }

            std::istringstream iss(line);
            std::string command, channel_name, user, file;
            iss >> command >> channel_name >> user >> file;

            auto it = channelMessages_.find(channel_name);
            if (it != channelMessages_.end())
            {
                const auto &messages = it->second;

                // Filter messages by user
                std::vector<std::string> userMessages;
                for (const auto &message : messages)
                {
                    if (message.find("user: " + user) != std::string::npos)
                    {
                        userMessages.push_back(message);
                    }
                }

                // Sort messages by date_time and event_name
                std::sort(userMessages.begin(), userMessages.end(), [](const std::string &a, const std::string &b)
                          {
                    std::string dateTimeA, eventNameA, dateTimeB, eventNameB;
                    std::istringstream issA(a), issB(b);
                    std::string line;

                    while (std::getline(issA, line)) {
                        if (line.find("date time: ") != std::string::npos) {
                            dateTimeA = line.substr(line.find("date time: ") + 11);
                        } else if (line.find("event name: ") != std::string::npos) {
                            eventNameA = line.substr(line.find("even name: ") + 12);
                        }
                    }

                    while (std::getline(issB, line)) {
                        if (line.find("date time: ") != std::string::npos) {
                            dateTimeB = line.substr(line.find("date time: ") + 11);
                        } else if (line.find("event name: ") != std::string::npos) {
                            eventNameB = line.substr(line.find("event name: ") + 12);
                        }
                    }

                    if (dateTimeA == dateTimeB) {
                        return eventNameA < eventNameB;
                    }
                    return dateTimeA < dateTimeB; });

                // Calculate total reports and active reports
                int totalReports = userMessages.size();
                int activeReports = std::count_if(userMessages.begin(), userMessages.end(), [](const std::string &message)
                                                  { return message.find("active: true") != std::string::npos; });

                int forcesArival = std::count_if(userMessages.begin(), userMessages.end(), [](const std::string &message)
                                                 { return message.find("forces arrival at scene: true") != std::string::npos; });

                // Write summary and sorted messages to file
                std::ofstream outFile(file);
                if (outFile.is_open())
                {
                    outFile << "Channel " << channel_name << ", Total: " << totalReports << ", active: " << activeReports << ", forces arrival at scene: " << forcesArival << std::endl;
                    for (auto &message : userMessages)
                    {
                        // Convert date_time from number to string
                        std::size_t dateTimePos = message.find("date time: ");
                        if (dateTimePos != std::string::npos)
                        {
                            std::string dateTimeStr = message.substr(dateTimePos + 11, 10);
                            std::time_t dateTime = std::stol(dateTimeStr);
                            std::tm *tm = std::localtime(&dateTime);
                            char buffer[20];
                            std::strftime(buffer, 20, "%d/%m/%y %H:%M", tm);
                            message.replace(dateTimePos + 11, 10, buffer);
                        }

                        // Convert description to summary
                        std::size_t descriptionPos = message.find("description: ");
                        if (descriptionPos != std::string::npos)
                        {
                            std::string description = message.substr(descriptionPos + 13);
                            std::string summary = description.substr(0, 27);
                            if (description.length() > 27)
                            {
                                summary += "...";
                            }
                            message.replace(descriptionPos, 12, "summary: ");
                            message.replace(descriptionPos + 9, description.length(), summary);
                        }

                        outFile << message << std::endl;
                    }
                    outFile.close();
                }
                else
                {
                    std::cout << "Unable to open file: " << file << std::endl;
                }
            }
        }
        else if (line.substr(0, 6) == "logout")
        {
            if (!loggedIn_)
            {
                std::cout << "You must be logged in to join a channel." << std::endl;
                continue;
            }

            std::istringstream iss(line);
            std::string reciept;
            iss >> reciept;

            StompFrame frame("DISCONNECT");
            frame.addHeader("accept-version", "1.2");
            frame.addHeader("reciept", reciept);

            std::string receiptId = connectionHandler_.generateReceiptId();
            frame.addHeader("receipt", receiptId);

            std::string frameStr = frame.toString();
            if (!connectionHandler_.sendFrameAscii(frameStr, '\0'))
            {
                std::cout << "Disconnected. Exiting...\n"
                          << std::endl;
                break;
            }

            addFrame(receiptId, frame);
            setLoggedIn(false);
        }
        else
        {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }
}

void KeyboardReader::addFrame(const std::string &receiptId, const StompFrame &frame)
{
    sentFrames_[receiptId] = frame;
}

void KeyboardReader::setLoggedIn(bool status)
{
    loggedIn_ = status;
}

StompFrame KeyboardReader::getFrame(const std::string &receiptId) const
{
    auto it = sentFrames_.find(receiptId);
    if (it != sentFrames_.end())
    {
        return it->second;
    }
    return StompFrame(""); // Return an empty frame if not found
}

void KeyboardReader::addMessageToChannel(const std::string &channel, const std::string &message)
{
    channelMessages_[channel].push_back(message);
}

void KeyboardReader::startSocketReader()
{
    if (!socketThread_.joinable())
    {
        SocketReader socketReader(*this);
        socketThread_ = std::thread(&SocketReader::run, socketReader);
    }
}

void KeyboardReader::stopSocketThread()
{
    if (socketThread_.joinable())
    {
        stopThread_ = true;
        socketThread_.join();
    }
}

ConnectionHandler &KeyboardReader::getConnectionHandler()
{
    return connectionHandler_;
}

bool KeyboardReader::getStopThread()
{
    return stopThread_;
}

void KeyboardReader::setStopThread(bool stopThread)
{
    stopThread_ = stopThread;
}
int KeyboardReader::generateSubscriptionId()
{
    return subscriptionIdCounter_++;
}
