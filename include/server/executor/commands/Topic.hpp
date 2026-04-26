#include "ACommand.hpp"

class Topic : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};