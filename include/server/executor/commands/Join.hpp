#include "ACommand.hpp"

class Join : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};