#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

class Client 
{
    private:
        int         _fd;
        std::string _nickname;
        std::string _username;
        //...

        /* buffer for incoming data from recv() */
        std::string _buffer;
    public:
        Client(int fd);
        //...
        
        int          getFd() const;
        std::string& getBuffer();
};

#endif 