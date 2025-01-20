#include "../include/StompFrame.h"

StompFrame::StompFrame(const std::string &command) : command_(command) {}

void StompFrame::addHeader(const std::string &key, const std::string &value)
{
    headers_[key] = value;
}

void StompFrame::setBody(const std::string &body)
{
    body_ = body;
}

std::string StompFrame::toString() const
{
    std::string frame = command_ + "\n";
    for (const auto &header : headers_)
    {
        frame += header.first + ":" + header.second + "\n";
    }
    frame += "\n" + body_ + "\0";
    return frame;
}