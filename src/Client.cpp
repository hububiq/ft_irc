#include "Client.hpp"

Client::Client(int fd): _fd(fd) {}

int Client::getFd() const
{
    return this->_fd;
}

std::string& Client::getBuffer()
{
    return this->_buffer;
}