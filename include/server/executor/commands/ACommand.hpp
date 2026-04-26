#ifndef COMMAND_HPP
#define COMMAND_HPP

class Client;
class Server;
struct Message;

class ACommand {
 public:
  virtual ~ACommand() {};
  virtual void execute(Client&, Message&) = 0;
};

#endif