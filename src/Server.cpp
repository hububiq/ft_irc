#include "Server.hpp" 
#include <iostream>
#include <cstdlib>
#include <stdexcept>

Server::Server(int argc, char **argv)
{
    parseArg(argc, argv);
}

void Server::parseArg(int argc, char **argv)
{
    if (argc != 3)
        throw std::invalid_argument("Please use format: ./irc <port> <password>");
    this->_port = std::atoi(argv[1]);
    if (this->_port <= 1024 || this->_port > 65535)
        throw std::out_of_range("Port must be between 1025 and 65535");
    this->_password = std::string(argv[2]);
    if (this->_password.empty())
        throw std::out_of_range("Password must be contain of at least 1 letter");
}

void Server::run()
{
    //for Szymon
}

Server::~Server()
{
    //cleaning
}