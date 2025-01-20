#ifndef STOMPFRAME_H
#define STOMPFRAME_H

#include <string>
#include <map>

class StompFrame
{
public:
    StompFrame(const std::string &command);
    void addHeader(const std::string &key, const std::string &value);
    void setBody(const std::string &body);
    std::string toString() const;

private:
    std::string command_;
    std::map<std::string, std::string> headers_;
    std::string body_;
};

#endif // STOMPFRAME_H