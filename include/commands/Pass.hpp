#include "ACommand.hpp"

class Pass: public ACommand
{
    public:
        void execute(Client&, Message&, Server&);
};