#ifndef SOCKETREADER_H
#define SOCKETREADER_H

#include "ConnectionHandler.h"
#include "KeyboardReader.h"

class KeyboardReader;

class SocketReader
{
public:
    SocketReader(KeyboardReader &keyboardReader);
    void run();
    void stop();

private:
    KeyboardReader &keyboardReader_;
};

#endif // SOCKETREADER_H