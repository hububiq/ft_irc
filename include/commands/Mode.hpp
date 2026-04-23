#include "ACommand.hpp"

class Mode: public ACommand
{
    public:
        void execute(Client&, Message&, Server&);
};