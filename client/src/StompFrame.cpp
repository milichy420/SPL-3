#include "../include/StompFrame.h"
#include <sstream>

StompFrame::StompFrame() : command_(""), headers_(), body_("") {}

StompFrame::StompFrame(const std::string &command) : command_(command), headers_(), body_("") {}

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

StompFrame StompFrame::fromString(const std::string &frameStr)
{
    std::istringstream stream(frameStr);
    std::string command;
    std::getline(stream, command);

    StompFrame frame(command);
    std::string line;
    while (std::getline(stream, line) && !line.empty())
    {
        auto delimiterPos = line.find(':');
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);
        frame.addHeader(key, value);
    }

    std::string body;
    while (std::getline(stream, line))
    {
        body += line + "\n";
    }
    frame.setBody(body);

    return frame;
}