#include "Kick.hpp"
#include "Replies.hpp"
#include "Parser.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Channel.hpp"

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

void Kick::execute(Client &client, Message &msg, Server &server)
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
        //TODO: must broadcast to the entire channel that person has been kicked
        //TODO: reason must be displayed !!!
            std::cout << nick << " kicked " << client_to_kick << " form " << channel_name << std::endl;
        else
         std::cout << Replies::getReply(ERR_USERNOTINCHANNEL, nick, channel_name, "");
    }
    else
        std::cerr << Replies::getReply(ERR_CHANOPRIVSNEEDED, nick, channel_name, "");
}

Kick globalKickCmd;