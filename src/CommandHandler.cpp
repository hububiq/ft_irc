//#include "CommandHandler.hpp"
//#include "ClientStatus.hpp"
//#include "Channel.hpp"
//#include "Parser.hpp"
#include "../include/CommandHandler.hpp"
#include "../include/Channel.hpp"
#include "../include/Replies.hpp"


#include <map>
#include <iostream>
#include <cctype>
#include <algorithm>

// void CommandHandler::handleQuit(Client& client, Message& msg, Server& server)
// {
//     //build reason and prefix
//     //notify and remove from all channels
//     //erase from server._clients using fd
//     //closing fd probably happens on server's side?
// }

void CommandHandler::handlePrivMsg(Client& client, Message& msg, Server& server)
{
    if (msg.params.empty() || msg.params[0].empty()) {
        std::cerr << "ERR_NORECIPIENT- log" << std::endl;
        return ;
    }
    if (msg.params.size() < 2 || msg.params[1].empty() || msg.params[1][0] != ':') {
        std::cerr << "ERR_NOTEXTTOSEND- log" << std::endl;
        return ;
    }
    Channel* channForMsg = server.getChannel(msg.params[0]); 
    if (channForMsg == NULL 
        && !Parser::findClient(server.getClients(), msg.params[0])) {
        std::cerr << "ERR_NOSUCHNICK - log" << std::endl;
        return ;
    }
    if (channForMsg != NULL) {    //PRIVMSG to channel
        std::string prefix = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
        std::string reply = prefix + " PRIVMSG " + channForMsg->getChannelName() + " " + msg.params[1] + "\r\n";
        channForMsg->broadcast(client, reply);
    }
    else {

    }
    //the same for PRIVMSG to client directly
}

void CommandHandler::handleJoin(Client& client, Message& msg, Server& server) {
    if (msg.params.empty()) {
        std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl;
        return ;
    }
    std::string& channelName = msg.params[0];
    if (!Parser::isValidChannelName(channelName)) {
        std::cerr << "ERR_NOSUCHCHANNEL - log" << std::endl;
        return ;
    }
    Channel* ch = server.getChannel(channelName);
    if (ch == NULL) {
        Channel channel = Channel(); //could be refactored with parametrised constructor
        server.addChannel(channel, channelName);
        server.getChannel(channelName)->getAdmins().push_back(&client);
        server.getChannel(channelName)->add_client(&client);
        std::cout << client.getRealName() 
            << " joined channel: " 
            << channelName 
            << " --- operator" << std::endl;
    }
    else if (ch != NULL) {
        if (Parser::modeGuardChecks(ch, msg))
            return ;
        ch->add_client(&client);
        std::cout << client.getRealName() << " joined channel: " << channelName << std::endl;
    }
}

bool clientCanKICK(Channel *channel_obj, Client &client)
{
    std::vector<Client *> temp_members = channel_obj->getAdmins();
    std::vector<Client*>::iterator it = temp_members.begin();
    std::vector<Client*>::iterator it_end = temp_members.end();
    while (it != it_end)
    {
        if (*it == &client)
            return true;
        ++it;
    }
    return false;
}
bool clientToBeKICKed(Channel *channel_obj, const std::string& client_to_kick)
{
    std::vector<Client *>& temp_members = channel_obj->getMembers();
    std::vector<Client*>::iterator it = temp_members.begin();
    std::vector<Client*>::iterator it_end = temp_members.end();

    while (it != it_end)
    {
        Client * tmp_mem = *it;
        if (tmp_mem->getNickname() == client_to_kick) {
            temp_members.erase(it);
            std::cout << "Found client to kick" << std::endl;
            return true;
        }
        ++it;
    }
    return false;
}

void CommandHandler::handleKick(Client &client, Message &msg, Server &server)
{
    // KICK <channel> <nick> [<reason>]
    if (msg.params.empty() || msg.params.size() < 2) {
        std::cerr << Replies::getReply(ERR_NEEDMOREPARAMS, client.getNickname(), "KICK", "");
        return ;
    }
    std::string nick = client.getNickname(); // nick of the KICKer
    std::string channel_name = msg.params[0];
    std::string client_to_kick = msg.params[1];
    std::cout << nick << " is looking to kick " << client_to_kick << std::endl;
    Channel *channel_obj = server.getChannel(channel_name);
    if (!channel_obj) {
        std::cout << Replies::getReply(ERR_NOSUCHCHANNEL, nick, channel_name, "");
        return;
    }
    if (clientCanKICK(channel_obj, client))
    {
        if (clientToBeKICKed(channel_obj, client_to_kick))
            std::cout << nick << " kicked " << client_to_kick << " form " << channel_name << std::endl;
        else
         std::cout << Replies::getReply(ERR_USERNOTINCHANNEL, nick, channel_name, "");
    }
    else
        std::cerr << Replies::getReply(ERR_CHANOPRIVSNEEDED, nick, channel_name, "");
}

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
    if (client.getAuthInfo()) {
		std::cout << client.getAuthInfo() << std::endl; 
        std::cerr << "462 ERR_ALREADYREGISTERED DUPA - log" << std::endl;
        return;
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
        commands["JOIN"] = handleJoin;
        commands["PRIVMSG"] = handlePrivMsg;
        commands["KICK"] = handleKick;
        // commands["QUIT"] = handleQuit;
    
    };
    std::map<std::string, void(*)(Client&, Message&, Server&)>::iterator it = commands.find(msg.command);
    if (it != commands.end())
    {
        it->second(client, msg, server);
    }
    else
        std::cerr << "ERR_COMMANDUNKNOWN - log" << std::endl;
}
