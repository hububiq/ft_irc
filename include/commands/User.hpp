#include "ACommand.hpp"

class User: public ACommand
{
    public:
        void execute(Client&, Message&, Server&);
};