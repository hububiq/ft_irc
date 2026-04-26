#include "Kick.hpp"

#include <algorithm>

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

bool Kick::clientCanKICK(Channel *channel_obj, Client &client) {
  std::vector<Client *> temp_members = channel_obj->getAdmins();
  std::vector<Client *>::iterator it = temp_members.begin();
  std::vector<Client *>::iterator it_end = temp_members.end();
  while (it != it_end) {
    if (*it == &client) return true;
    ++it;
  }
  return false;
}

Client *Kick::clientToBeKICKed(Channel *channel_obj,
                               const std::string &client_to_kick) {
  std::vector<Client *> &temp_members = channel_obj->getMembers();
  for (std::vector<Client *>::iterator it = temp_members.begin();
       it != temp_members.end(); ++it) {
    if ((*it)->getNickname() == client_to_kick) return *it;
  }
  return NULL;
}

void Kick::execute(Client &client, Message &msg, Server &server) {
  // KICK <channel> <nick> [<reason>]
  if (msg.params.empty() || msg.params.size() < 2) {
    std::cerr << Replies::getReply(ERR_NEEDMOREPARAMS, client.getNickname(),
                                   "KICK", "");
    return;
  }
  std::string nick = client.getNickname();  // nick of the KICKer
  std::string channel_name = msg.params[0];
  std::string client_to_kick = msg.params[1];
  std::string reason =
      (msg.params.size() > 2) ? msg.params[2] : "No reason given";
  Channel *channel_obj = server.getChannel(channel_name);
  if (!channel_obj) {
    std::cout << Replies::getReply(ERR_NOSUCHCHANNEL, nick, channel_name, "");
    return;
  }
  if (Kick::clientCanKICK(channel_obj, client)) {
    // reminder: func clientToBEKICKed erases the client.
    Client *kicked = Kick::clientToBeKICKed(channel_obj, client_to_kick);
    if (kicked) {
      std::string replay = ":" + nick + "!" + client.getRealName() +
                           "@localhost" + " KICK " + channel_name + " " +
                           client_to_kick + " :" + reason + "\r\n";
      kicked->write_msg(replay);  // this line notifies the kicked clinet first
                                  // (that is kicked)
      channel_obj->broadcast(client, replay);
      // find and earse
      std::vector<Client *> &members = channel_obj->getMembers();
      members.erase(std::find(members.begin(), members.end(), kicked));
      // clear if client admin
      std::vector<Client *> &admins = channel_obj->getAdmins();
      std::vector<Client *>::iterator it_a =
          std::find(admins.begin(), admins.end(), kicked);
      if (it_a != admins.end()) admins.erase(it_a);
    } else {
      std::string err =
          Replies::getReply(ERR_USERNOTINCHANNEL, nick, channel_name, "");
      client.write_msg(err);
    }
  } else {
    std::string err =
        Replies::getReply(ERR_CHANOPRIVSNEEDED, nick, channel_name, "");
    client.write_msg(err);
  }
}
Kick globalKickCmd;
