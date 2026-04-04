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
        throw std::out_of_range("Password must contain of at least 1 letter");
}

/* ------------------ belongs to Hubert ------------------*/
void Server::executeCommand(/*Client* client, */const Message& message)
{
    //just testing Message struct
    std::cout << message.prefix + " ";
    std::cout << message.command << std::endl;
    for (size_t i = 0; i < message.params.size(); i++)
        std::cout << message.params[i] << std::endl;
    std::cout << "Sanity check for vector of strings size: " << message.params.size() << std::endl;
}

/*------------------ for Szymon ------------------
Source: https://deepwiki.com/42YerevanProjects/ft_irc/3-command-processing-system
When a client sends a message to the server, the following process occurs:
1. The server receives the message and passes it to the Parser's method
2. The Parser tokenizes the message into a command name and arguments (struct Message in our case)
*/
bool Server::processReceivedData(int client_fd) 
{
    (void)client_fd;
    //read with recv() until \r\n
    //use Client's getBuffer()
    //append data chunks to Client's buffer
    //erase the buffer up to \r\n, leave the rest in the buffer for the next loop
    //send this buffer as a messagLine (std::string) to parser.
    //also: create client's object to pass for command execution


    //Hardcoded message for parsing
    std::string messageLine = ":dupa #shouldbecommandhere lol :hehe";
    Message msg;
    if (!messageLine.empty())
    {
        Parser::parse(messageLine, msg);
        this->executeCommand(/*client, */msg);
    }
    return true;
}

/* ------------------ belongs to Hubert ------------------ */
void Server::run()
{
    /*
    setup sockets, pollfds array etc.

    get client_fd for Hubert for parsing

    call processReceivedData(client_fd)
    ...
    */
    processReceivedData(10000);
}

Server::~Server()
{
    //cleaning
}