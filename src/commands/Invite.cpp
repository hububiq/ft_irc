#include "Invite.hpp"
#include "Replies.hpp"
#include "Parser.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"

void Invite::execute(Client& client, Message& msg, Server& server)
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
    Channel* ch = server.getChannel(msg.params[1]);
    if (!ch) {
        std::string reply = Replies::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], msg.params[1]);
        client.write_msg(reply);
        return ;
    }
    if (ch && !Parser::isUserInChannel(server, client, ch->getChannelName())) {
        std::string reply = Replies::getReply(ERR_NOTONCHANNEL, nickname, msg.params[1], "");
        client.write_msg(reply);
        return ;
    }
    if (ch && Parser::isInviteeInChannel(server, msg, ch->getChannelName())) {
        std::string reply = Replies::getReply(ERR_USERONCHANNEL, nickname, msg.params[0], msg.params[1]);
        client.write_msg(reply);
        return ;
    }
    if (!Parser::isValidChannelName(msg.params[1])) {
        std::string reply = Replies::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], "");
        client.write_msg(reply);
        return ;
    }
    if (ch && ch->isInviteOnly()) {
        if (!Parser::isClientAdmin(server, client, msg.params[1])) {
            std::string reply = Replies::getReply(ERR_CHANOPRIVSNEEDED, nickname, msg.params[0], "");
            client.write_msg(reply);
            return ;
        }
    }
    ch->addToInvited(msg.params[0]);
    std::string reply = Replies::getReply(RPL_INVITING, nickname, msg.params[0], msg.params[1]);
    client.write_msg(reply);
    std::map<int, Client>& cl = server.getClients();
    std::map<int, Client>::iterator it;
    std::string prefix = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
    for (it = cl.begin(); it != cl.end(); it++) {
        if (it->second.getNickname() == msg.params[0]) {
            std::string reply = prefix + " INVITE " + it->second.getNickname() + " :" + msg.params[1] + "\r\n";
            it->second.write_msg(reply);
        }
    }
}

Invite globalInviteCmd;