#include "CommandHandler.hpp"
#include "Replies.hpp"
#include "Server.hpp"

void Server::executeMessage(Client& client, Message& msg, Server& server) {
  switch (client.getState()) {
    case CONNECTED: {
      std::cout << "inside CONNECTED: " << msg.command << std::endl;
      if (msg.command == "CAP" || msg.command == "PASS") {
        break;
      }
      std::cerr << "Login reguired: PASS" << std::endl;
      std::string reply = Replies::getReply(ERR_NOTREGISTERED, "*", "", "");
      client.write_msg(reply);
      return;
    }
    case HANDSHAKE: {
      std::cout << "inside HANDSHAKE: " << msg.command << std::endl;
      if (msg.command == "NICK" || msg.command == "USER" ||
          msg.command == "CAP") {
        break;
      } else {
        std::string reply = Replies::getReply(ERR_NOTREGISTERED, "*", "", "");
        client.write_msg(reply);
        return;
      }
    } break;
    case REGISTERED: {
      std::cout << "inside REGISTERED: " << msg.command << std::endl;
      if (msg.command == "PASS" || msg.command == "USER" ||
          msg.command == "NICK") {
        std::string reply = Replies::getReply(ERR_ALREADYREGISTERED,
                                              client.getNickname(), "", "");
        client.write_msg(reply);
        return;
      }
      break;
    }
  }
  CommandHandler::handleCommands(client, msg, server);
}
