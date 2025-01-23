#ifndef KEYBOARDREADER_H
#define KEYBOARDREADER_H

#include "ConnectionHandler.h"
#include "StompFrame.h"
#include <map>
#include <string>
#include <vector>

class KeyboardReader
{
public:
    KeyboardReader(ConnectionHandler &connectionHandler);
    void run();
    void addFrame(const std::string &receiptId, const StompFrame &frame);
    void setLoggedIn(bool status);
    StompFrame getFrame(const std::string &receiptId) const;
    void addMessageToChannel(const std::string &channel, const std::string &message);

private:
    ConnectionHandler &connectionHandler_;
    std::map<std::string, StompFrame> sentFrames_;
    std::map<std::string, std::vector<std::string>> channelMessages_;
    bool loggedIn_;
    std::string user_;
};

#endif // KEYBOARDREADER_H