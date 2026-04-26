#include <string>

#include "ACommand.hpp"

class Channel;
class Client;

class Kick : public ACommand {
 public:
  void execute(Client &, Message &, Server &);
  bool clientCanKICK(Channel *, Client &);
  Client *clientToBeKICKed(Channel *, const std::string &);
};

// Client *clientToBeKICKed(Channel *, const std::string &);
// bool clientCanKICK(Channel *, Client &);
