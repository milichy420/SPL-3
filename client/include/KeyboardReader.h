#ifndef KEYBOARDREADER_H
#define KEYBOARDREADER_H

#include "ConnectionHandler.h"
#include "StompFrame.h"

class KeyboardReader
{
public:
    KeyboardReader(ConnectionHandler &connectionHandler);
    void run();
    void addFrame(const std::string &receiptId, const StompFrame &frame);

private:
    ConnectionHandler &connectionHandler_;
    std::map<std::string, StompFrame> sentFrames_;
};

#endif // KEYBOARDREADER_H