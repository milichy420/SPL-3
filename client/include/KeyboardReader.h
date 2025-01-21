#ifndef KEYBOARDREADER_H
#define KEYBOARDREADER_H

#include "ConnectionHandler.h"
#include "StompFrame.h"
#include <map>
#include <string>

class KeyboardReader
{
public:
    KeyboardReader(ConnectionHandler &connectionHandler);
    void run();
    void addFrame(const std::string &receiptId, const StompFrame &frame);
    void setLoggedIn(bool status);
    StompFrame getFrame(const std::string &receiptId) const;

private:
    ConnectionHandler &connectionHandler_;
    std::map<std::string, StompFrame> sentFrames_;
    bool loggedIn_;
};

#endif // KEYBOARDREADER_H