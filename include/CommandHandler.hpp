#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP
# include "Message.hpp"
# include "Client.hpp"
# include "Server.hpp"

namespace CommandHandler 
{
    void handleCommand(Client& client, Message& msg, Server& server);
    void handlePass(Client& client, Message& msg, Server& server);
    // void handleNick(Client& client, Message& msg, Server& server);
};

#endif 