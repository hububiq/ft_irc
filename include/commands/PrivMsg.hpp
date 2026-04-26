#include "ACommand.hpp"

class PrivMsg : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};