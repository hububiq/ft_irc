#include "ACommand.hpp"

class Quit: public ACommand
{
public:
    void execute(Client&, Message&, Server&);
};
