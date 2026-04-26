#include "ACommand.hpp"

class Cap : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};