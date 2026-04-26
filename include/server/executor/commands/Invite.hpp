#include "ACommand.hpp"

class Invite : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};