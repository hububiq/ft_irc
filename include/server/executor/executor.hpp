#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "Client.hpp"
#include "ClientState.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "command_handler.hpp"
#include "reply_factory.hpp"

const std::string CMD_CAP = "CAP";
const std::string CMD_INVITE = "INVITE";
const std::string CMD_JOIN = "JOIN";
const std::string CMD_KICK = "KICK";
const std::string CMD_MODE = "MODE";
const std::string CMD_NICK = "NICK";
const std::string CMD_PASS = "PASS";
const std::string CMD_PING = "PING";
const std::string CMD_PRIVMSG = "PRIVMSG";
const std::string CMD_QUIT = "QUIT";
const std::string CMD_TOPIC = "TOPIC";
const std::string CMD_USER = "USER";

class CommandHandler;

class Executor {
public:
  Executor(CommandHandler *commandHandler);
  ~Executor();
  void executeMessage(Client &client, Message &msg);

private:
  CommandHandler *m_commandHandler;
};

#endif