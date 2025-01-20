#ifndef SOCKETREADER_H
#define SOCKETREADER_H

#include "ConnectionHandler.h"

class SocketReader
{
public:
    SocketReader(ConnectionHandler &connectionHandler);
    void run();

private:
    ConnectionHandler &connectionHandler_;
};

#endif // SOCKETREADER_H