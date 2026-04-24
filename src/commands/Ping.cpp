#include "Ping.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"

void Ping::execute(Client& client, Message& msg, Server& server)
{
    if (msg.params.empty()) {
        std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
        client.write_msg(reply);
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

Ping globalPingCmd;