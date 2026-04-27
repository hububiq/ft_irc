#ifndef EXECUTOR_COMMANDS_MODE_HPP
#define EXECUTOR_COMMANDS_MODE_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "mode_reporter.hpp"
#include "reply_factory.hpp"

class ModeReporter;

class Mode : public ACommand {
 private:
  ModeReporter* m_modeReporter;
 public:
  Mode(ServerDao *server = NULL, ModeReporter *modeReporter = NULL);
  
  
  
  void execute(Client&, Message&);
};

#endif
