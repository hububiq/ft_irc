#include "ACommand.hpp"

class Ping: public ACommand
{
    public:
        void execute(Client&, Message&, Server&);
};