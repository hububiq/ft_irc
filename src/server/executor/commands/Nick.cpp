#include "Nick.hpp"



void Nick::execute(Client& client, Message& msg) {
  if (msg.params.empty()) {
    std::string reply =
        reply_factory::getReply(ERR_NONICKNAMEGIVEN, "*", "", "");
    client.write_msg(reply);
    return;
  }
  if (!m_validator->isValidNickname(msg.params[0], client)) {
    return;
  }
  std::map<int, Client>& cliMap = m_server->getClients();
  std::map<int, Client>::iterator it;
  for (it = cliMap.begin(); it != cliMap.end(); it++) {
    if (it->second.getNickname() == msg.params[0]) {
      std::string reply =
          reply_factory::getReply(ERR_NICKNAMEINUSE, "*", msg.params[0], "");
      client.write_msg(reply);
      return;
    }
  }
  client.setNickname(msg.params[0]);
}







Nick::Nick(ServerDao *server, Validator *validator) : ACommand(server), m_validator(validator) {}
