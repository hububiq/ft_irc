#include "ACommand.hpp"

class Nick : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};