#ifndef KEYBOARDREADER_H
#define KEYBOARDREADER_H

#include "ConnectionHandler.h"
#include "SocketReader.h"
#include "StompFrame.h"
#include <map>
#include <string>
#include <vector>
#include <thread>

class SocketReader;

class KeyboardReader
{
public:
    // KeyboardReader(ConnectionHandler &connectionHandler);
    // KeyboardReader(ConnectionHandler connectionHandler);
    KeyboardReader();
    ~KeyboardReader();
    void run();
    void sendFrame(std::string &frameStr, int &retFlag);
    void addFrame(const std::string &receiptId, const StompFrame &frame);
    void setLoggedIn(bool status);
    StompFrame getFrame(const std::string &receiptId) const;
    void addMessageToChannel(const std::string &channel, const std::string &message);
    ConnectionHandler &getConnectionHandler();
    void startSocketReader();
    void stopSocketThread();
    bool sendFrame(const StompFrame &frame);
    void setStopThread(bool stopThread);
    bool getStopThread();

private:
    // ConnectionHandler &connectionHandler_;
    ConnectionHandler connectionHandler_;
    std::map<std::string, StompFrame> sentFrames_;
    std::map<std::string, std::vector<std::string>> channelMessages_;
    bool loggedIn_;
    std::string user_;
    std::thread socketThread_;
    bool stopThread_;
};

#endif // KEYBOARDREADER_H