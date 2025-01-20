#ifndef KEYBOARDREADER_H
#define KEYBOARDREADER_H

#include "ConnectionHandler.h"

class KeyboardReader
{
public:
    KeyboardReader(ConnectionHandler &connectionHandler);
    void run();

private:
    ConnectionHandler &connectionHandler_;
};

#endif // KEYBOARDREADER_H