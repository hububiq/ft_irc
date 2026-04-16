//#include "CommandHandler.hpp"
//#include "ClientStatus.hpp"
//#include "Channel.hpp"
#include "Parser.hpp"
#include "../include/CommandHandler.hpp"
#include "../include/Channel.hpp"
#include "../include/Replies.hpp"


#include <map>
#include <iostream>
#include <cctype>
#include <algorithm>

void CommandHandler::handleTopic(Client& client, Message& msg, Server& server)
{
    std::string nickname = client.getNickname();
    std::string command = msg.command;
    bool isUserInChan = Parser::isUserInChannel(server, client);
    if (msg.params.empty()) {
        std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
        client.write_msg(reply);
        return ;
    }
    Channel* chan = server.getChannel(msg.params[0]);
    if (!chan) {
        std::string reply = Replies::getReply(ERR_NOTONCHANNEL, nickname, msg.params[0], "");
        client.write_msg(reply);
        return ;
    }
    if (!isUserInChan) {
        std::string reply = Replies::getReply(ERR_NOTONCHANNEL, nickname, msg.params[0], "");
        client.write_msg(reply);
        return ;
    }
    if (chan && isUserInChan && msg.params.size() == 1) { //there is only one parameter and user is in channel
        if (chan->isThereTopic()) {
            std::string reply = Replies::getReply(RPL_TOPIC, nickname, msg.params[0], chan->getTopic());
            client.write_msg(reply);
        }
        else {
            std::string reply = Replies::getReply(RPL_NOTOPIC, nickname, msg.params[0], "");
            client.write_msg(reply);
        }
        return ;
    }
    if (isUserInChan && msg.params.size() >= 2) {
        Channel* tempChan = server.getChannel(msg.params[0]);
        if (!tempChan) { 
            std::string reply = Replies::getReply(ERR_NOTONCHANNEL, nickname, msg.params[0], "");
            client.write_msg(reply);
            return ;
        }
        if (tempChan && tempChan->isTopicForOperator() && !(Parser::isClientAdmin(server, client, msg.params[0]))) {
            std::string reply = Replies::getReply(ERR_CHANOPRIVSNEEDED, nickname, msg.params[0], "");
            client.write_msg(reply);
            return ;
        }
        
        //no flag "topic only by operators" - everybody can assign topic if its not empty
        if (tempChan && !msg.params[1].empty())
                tempChan->setTopic(msg.params[1]);
        if (!tempChan->getTopic().empty()) {
            std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
                + "@" + client.getHostname() + " TOPIC " + tempChan->getChannelName() + " :" + tempChan->getTopic() + "\r\n";
            tempChan->broadcast(client, reply);
            client.write_msg(reply); //needed - TOPIC is also viewed for user-setter
        }
    }
}


// void CommandHandler::handleQuit(Client& client, Message& msg, Server& server)
// {
//     //build reason and prefix
//     //notify and remove from all channels
//     //erase from server._clients using fd
//     //closing fd probably happens on server's side?
// }

void CommandHandler::handleInvite(Client& client, Message& msg, Server& server)
{
    std::string nickname = client.getNickname();
    std::string command = msg.command;
    if (msg.params.size() < 2) {
        std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
        client.write_msg(reply);
        return ;
    }
    if (!Parser::findClient(server.getClients(), msg.params[0])) {
        std::string reply = Replies::getReply(ERR_NOSUCHNICK, nickname, msg.params[1], "");
        client.write_msg(reply);
        return ;
    }
    if (!Parser::isUserInChannel(server, client)) {
        std::string reply = Replies::getReply(ERR_NOTONCHANNEL, nickname, msg.params[1], "");
        client.write_msg(reply);
        return ;
    }
    if (Parser::isInviteeInChannel(server, msg)) {
        std::string reply = Replies::getReply(ERR_USERONCHANNEL, nickname, msg.params[0], msg.params[1]);
        client.write_msg(reply);
        return ;
    }
    if (!Parser::isValidChannelName(msg.params[1])) {
        std::string reply = Replies::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], "");
        client.write_msg(reply);
        return ;
    }
    Channel* ch = server.getChannel(msg.params[1]);
    if (ch && ch->isInviteOnly()) {
        if (!Parser::isClientAdmin(server, client, msg.params[1])) {
                std::string reply = Replies::getReply(ERR_CHANOPRIVSNEEDED, nickname, msg.params[0], "");
                client.write_msg(reply);
                return ;
            }
    }
    ch->addToInvited(&client);
    std::string reply = Replies::getReply(RPL_INVITING, nickname, msg.params[0], msg.params[1]);
    client.write_msg(reply);
    std::map<int, Client>& cl = server.getClients();
    std::map<int, Client>::iterator it;
    std::string prefix = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
    for (it = cl.begin(); it != cl.end(); it++) {
        if (it->second.getNickname() == msg.params[0]) {
            std::string reply = prefix + " " + command + " " + it->second.getNickname() + " :" + msg.params[1] + "\r\n";
            it->second.write_msg(reply);
        }
    }
}

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
    std::string prefix = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
    if (channForMsg != NULL) {    //PRIVMSG to channel
        std::string reply = prefix + " PRIVMSG " + channForMsg->getChannelName() + " " + msg.params[1] + "\r\n";
        channForMsg->broadcast(client, reply);
    }
    else if (msg.params[0][0] != '#') { //PRIVMSG to client
        std::map<int, Client>& cl = server.getClients();
        std::map<int, Client>::iterator it;
        for (it = cl.begin(); it != cl.end(); it++) {
            if (it->second.getNickname() == msg.params[0]) {
                std::string reply = prefix + " PRIVMSG " + it->second.getNickname() + " " + msg.params[1] + "\r\n";
                it->second.write_msg(reply);
            }
        }
    }
}

/*TODO: check with Halloy if I need to sent RL_NOTOPIC if user joins canal without topic*/
/*      broadcast when somebody is joining*/
void CommandHandler::handleJoin(Client& client, Message& msg, Server& server) 
{
    std::string nickname = client.getNickname();
    std::string command = msg.command;
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
    if (ch && Parser::isUserInChannel(server, client)) {
        return; // case where user is on the channel - just ignore JOIN command
    }
    if (ch == NULL) { //channel will be freshly made so it has no topic - we dont send anythin to client which is joining
        Channel channel = Channel(); //could be refactored with parametrised constructor
        server.addChannel(channel, channelName);
        server.getChannel(channelName)->getAdmins().push_back(&client);
        server.getChannel(channelName)->add_client(&client);
    }
    else if (ch != NULL) {
        if (Parser::modeGuardChecks(ch, msg, client))
            return ;
        ch->add_client(&client);
        std::string joinReply = ":" + client.getNickname() + "!" + client.getUsername()
                + "@" + client.getHostname() + " JOIN " + " :" + ch->getChannelName() + "\r\n";
        ch->broadcast(client, joinReply);
        client.write_msg(joinReply);
        //ch->broadcast(client, reply) TODO here: add reply about joining to entire chanel
        if (ch->isThereTopic()) { //client succesfully joined, passed key if any set, now he will have viewed the channel topic
            std::string reply = Replies::getReply(RPL_TOPIC, nickname, msg.params[0], ch->getTopic());
            client.write_msg(reply);
            return ;
        }
        std::string reply = Replies::getReply(RPL_NOTOPIC, nickname, msg.params[0], ""); //in case of no topic, information about it is shown to client
        client.write_msg(reply);
		server.respond(client);
    }
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
	std::string reason = msg.params[2];
    std::cout << nick << " is looking to kick " << client_to_kick << std::endl;
    Channel *channel_obj = server.getChannel(channel_name);
    if (!channel_obj) {
        std::cout << Replies::getReply(ERR_NOSUCHCHANNEL, nick, channel_name, "");
        return;
    }
    if (Parser::clientCanKICK(channel_obj, client))
    {
        if (Parser::clientToBeKICKed(channel_obj, client_to_kick))
		{
			// TODO: to entire channel that person has been kicked [reason]
            std::cout << nick << " kicked " << client_to_kick << " form " << channel_name << std::endl;
			std::string replay = ":" + nick + "!" + client.getRealName() 
									+ "@" + "localhost" + " KICK " + channel_name 
									+ client_to_kick + reason;
			std::cout << replay; // <--this is test
			channel_obj->broadcast(client, replay);
        }
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
        || username.find_first_of(std::string(" \r\n\0", 5)) != std::string::npos
        || realname[0] != ':'
        || realname.find_first_of(std::string("\0\r\n", 4)) != std::string::npos) {
        std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl;
        return ;
    }
    client.setUsername(username);
    client.setRealName(realname.substr(1));
    client.setState(REGISTERED);
    client.setRegister();
    std::string reply = Replies::getReply(RPL_WELCOME, client.getNickname(), client.getUsername(), client.getHostname());
    client.write_msg(reply);
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

void CommandHandler::handlePing(Client &client, Message &msg, Server& server)
{
    if (msg.params.empty()) {
        std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl; 
        return ;
    }
    (void)server;
    if (!msg.params.empty()) {
        std::string serverName = "localhost";
        std::string prefix = ":" + serverName + " ";
        std::string replay = prefix + " PONG " + serverName + " : " + msg.params[0] + "\r\n";
        client.write_msg(replay);
    }
}
void CommandHandler::handleCap(Client& client, Message& msg, Server& server)
{
    (void)server;
    if (msg.params.empty()) {
        std::cerr << "Need more paramters - log " << std::endl;
        return ;
    }
    std::string serverName = "localhost";
    std::string prefix = ":" + serverName + " ";
    std::string replay = prefix + " CAP * " + msg.params[0] + " :\r\n";
    client.write_msg(replay);
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
        commands["INVITE"] = handleInvite;
        commands["PING"] = handlePing;
        commands["CAP"] = handleCap;
        commands["TOPIC"] = handleTopic;
        // commands["QUIT"] = handleQuit;
    
    };
    std::map<std::string, void(*)(Client&, Message&, Server&)>::iterator it = commands.find(msg.command);
    if (it != commands.end())
        it->second(client, msg, server);
    else
	{

		std::cout << msg.command << std::endl;
        std::cerr << "ERR_COMMANDUNKNOWN - log" << std::endl;
	}
}
