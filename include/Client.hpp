#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include "ClientStatus.hpp"

class Client
{
    private:
        int          _fd;
        std::string  _nickname;
        std::string  _username;
        ClientStatus _status;

        std::string _request_buffer;
        std::string _response_buffer;
    public:
        Client(int fd);

        int          getFd() const;
        std::string& getRequestBuffer();
        std::string& getResponseBuffer();
        ClientStatus getStatus() const;
        void         setStatus(ClientStatus status);
        void         reset();
};

#endif