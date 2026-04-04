#ifndef SERVER_HPP
# define SERVER_HPP
# include "Client.hpp"
# include "Parser.hpp"
# include "Message.hpp"
# include <string>
# include <map>
# include <iostream>

class Server 
{
    private:
        int                    _port;
        std::string            _password;
        std::map<int, Client*> _clients;
        void                   parseArg(int argc, char **argv);
        void                   executeCommand(/*Client* client, */const Message& message);  //belongs to semantic parsing 


        /* ------ processReceivedData BELONGS TO SZYMON
        function for receiving from socket with recv() and sending raw message to parser */
        bool                   processReceivedData(int client_fd);
    public:
        Server(int argc, char **argv);
        ~Server();
        void run();
};

#endif