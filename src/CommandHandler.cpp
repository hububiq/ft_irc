#include "CommandHandler.hpp"
#include "ClientStatus.hpp"
#include "Parser.hpp"

#include <map>
#include <iostream>
#include <cctype>
#include <algorithm>

void CommandHandler::handleUser(Client& client, Message& msg, Server& server)
{
    (void)server;
    if (msg.params.size() < 2) {
        std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl;
        return ;
    }
    std::string username = msg.params[0];
    std::string realname = msg.params[1];
    if (std::find_if(username.begin(), username.end(), ::isalpha) == username.end()
        || username.find_first_of(std::string(" \r\n\0", 4)) != std::string::npos
        || realname[0] != ':'
        || realname.find_first_of(std::string("\0\r\n", 3)) != std::string::npos) {
        std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl;
        return ;
    }
    client.setRealName(realname.substr(1));
    client.setState(REGISTERED);
    client.setRegister();
    std::cout << client.getState() << ": enum state" << std::endl;
    std::cout << client.getRealName() << ": this is realname" << std::endl;
    std::cout << client.getRegisterInfo() << " -- 1 if registered" << std::endl;
}

void CommandHandler::handleNick(Client& client, Message& msg, Server& server)
{
    if (msg.params.empty()) {
        std::cerr << "ERR_NONICKNAMEGIVEN - log" << std::endl; 
        return ;
    }
    if (!Parser::isValidNickname(msg.params[0])) {
        std::cerr << "ERR_ERRONEUSNICKNAME - log" << std::endl;
        return ;
    }
    std::map<int, Client >& cliMap = server.getClients();
    std::map<int, Client >::iterator it;
    for (it = cliMap.begin(); it != cliMap.end(); it++) {
        if (it->second.getNickname() == msg.params[0]) {
            std::cerr << "ERR_NICKNAMEINUSE - log" << std::endl;
            return ;
        }
    }
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
        commands["USER"] = handleUser;
    
    };
    std::map<std::string, void(*)(Client&, Message&, Server&)>::iterator it = commands.find(msg.command);
    if (it != commands.end())
    {
        it->second(client, msg, server);
    }
    else
        throw std::runtime_error("ERR_COMMANDUNKNOWN");
}