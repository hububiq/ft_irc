#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>

/*for Szymon*/
class Server 
{
    private:
        int             _port;
        std::string     _password;
        void            parseArg(int argc, char **argv);
    public:
        Server(int argc, char **argv);
        ~Server();

        void run();
};

#endif