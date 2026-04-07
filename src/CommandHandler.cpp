#include "CommandHandler.hpp"
#include "ClientStatus.hpp"
#include "Parser.hpp"
#include <map>
#include <iostream>

void CommandHandler::handleNick(Client& client, Message& msg, Server& server)
{
    (void)server;
    if (msg.params.empty()) {
        std::cerr << "ERR_NONICKNAMEGIVEN - log" << std::endl; 
        return ;
    }
    if (!Parser::isValidNickname(msg.params[0]))
        std::cerr << "ERR_ERRONEUSNICKNAME - log" << std::endl;
    client.setNickname(msg.params[0]);    
}

void CommandHandler::handlePass(Client& client, Message& msg, Server& server)
{
    if (msg.params.empty()) {
        std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl; 
        return ;
    }
    if ((msg.params[0] != server.getPassword())) {
        std::cerr << "ERR_PASSWDMISMATCH - log" << std::endl;
        return ;
    }
    client.setAuth();
    client.setState(HANDSHAKE);
}

void CommandHandler::handleCommand(Client& client, Message& msg, Server& server)
{
    static std::map<std::string, void(*)(Client&, Message&, Server&)> commands; 
    if (commands.empty())
    {
        commands["PASS"] = handlePass;
        commands["NICK"] = handleNick;
        //placeholder for other commands' pairs.
    
    };
    std::map<std::string, void(*)(Client&, Message&, Server&)>::iterator it = commands.find(msg.command);
    if (it != commands.end())
    {
        it->second(client, msg, server);
    }
    else
        throw std::runtime_error("ERR_COMMANDUNKNOWN");
}