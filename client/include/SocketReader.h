#ifndef SOCKETREADER_H
#define SOCKETREADER_H

#include "ConnectionHandler.h"
#include "KeyboardReader.h"

class SocketReader
{
public:
    SocketReader(ConnectionHandler &connectionHandler, KeyboardReader &keyboardReader);
    void run();

private:
    ConnectionHandler &connectionHandler_;
    KeyboardReader &keyboardReader_;
};

#endif // SOCKETREADER_H