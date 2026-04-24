#include "Mode.hpp"
#include "Replies.hpp"
#include "Parser.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"

void Mode::execute(Client& client, Message& msg, Server& server)
{
    std::string nickname = client.getNickname();
    std::string command = msg.command;
    if (msg.params.empty()) {
        std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
        client.write_msg(reply);
        return ;
    }
    if (msg.params.size() == 1) {
        if (server.getChannel(msg.params[0]) == NULL) {
            std::string reply = Replies::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], "");
            client.write_msg(reply);
            return ;
        }
        if (Parser::isClientAdmin(server, client, msg.params[0]))
            Parser::loopOnFlags(msg.params[0], server, client);
        else {
            std::string reply = Replies::getReply(ERR_CHANOPRIVSNEEDED, nickname, msg.params[0], "");
            client.write_msg(reply);
            return ;
        }
    }
    if (msg.params.size() >= 2) {
        Channel* chanForMode = server.getChannel(msg.params[0]);
        if (!chanForMode) {
            std::string reply = Replies::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], "");
            client.write_msg(reply);
            return ;
        }
        std::string& chName = chanForMode->getChannelName();
        if (Parser::isClientAdmin(server, client, chName)) {
            if (msg.params[1][0] == '+')
                chanForMode->setFlagOn(server, client, msg);
            else if (msg.params[1][0] == '-')
                chanForMode->setFlagOff(client, msg);
            else {
                std::string reply = Replies::getReply(ERR_UNKNOWNMODE, nickname, std::string(1, msg.params[1][0]), "");
                client.write_msg(reply);
            }
        }
        else {
            std::string reply = Replies::getReply(ERR_CHANOPRIVSNEEDED, nickname, chName, "");
            client.write_msg(reply);
        }
    }
}


Mode globalModeCmd;