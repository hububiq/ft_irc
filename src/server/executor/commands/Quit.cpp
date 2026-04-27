#include "Quit.hpp"



void Quit::execute(Client &client, Message &msg) {
  std::map<std::string, Channel> &channels = m_server->getChannels();
  std::map<std::string, Channel>::iterator it_channel_b = channels.begin();
  std::map<std::string, Channel>::iterator it_channel_e = channels.end();
  while (it_channel_b != it_channel_e) {
    std::vector<Client *> &clients =
        it_channel_b->second
            .getMembers();  // for each channel I go throught list of clients
    std::vector<Client *>::iterator it_client = std::find(
        clients.begin(), clients.end(), &client);  // &client to erase()
    if (it_client != clients.end()) {
      std::string replay = ":" + client.getNickname() + "!" +
                           client.getUsername() + "@" + client.getHostname() +
                           " QUIT :";
      if (!msg.params.empty() && !msg.params[0].empty())
        replay += msg.params[0];
      replay += "\r\n";
      // broadcast
      it_channel_b->second.broadcast(client, replay);
      clients.erase(it_client);
    }
    std::vector<Client *> &admins = it_channel_b->second.getAdmins();
    std::vector<Client *>::iterator it_admin =
        std::find(admins.begin(), admins.end(), &client);
    if (it_admin != admins.end()) admins.erase(it_admin);
    // check if the channel is empty, if yes erase() channel
    if (it_channel_b->second.getMembers().empty())
      channels.erase(it_channel_b++);
    else
      it_channel_b++;
  }
}






Quit::Quit(ServerDao *server) : ACommand(server) {}
