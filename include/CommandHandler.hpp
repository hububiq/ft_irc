#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP
# include "Message.hpp"
# include "Client.hpp"
# include "Server.hpp"

namespace CommandHandler 
{
    void handleCommand(Client& client, Message& msg, Server& server);
    void handlePass(Client& client, Message& msg, Server& server);
    void handleNick(Client& client, Message& msg, Server& server);
    void handleUser(Client& client, Message& msg, Server& server);
    void handleJoin(Client& client, Message& msg, Server& server);
    void handlePrivMsg(Client& client, Message& msg, Server& server);
    void handleKick(Client& client, Message& msg, Server& server);
    // void handleQuit(Client& client, Message& msg, Server& server);
};

#endif 
