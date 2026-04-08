#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

#include "Client.hpp"

class Channel {
    private:
        std::string _channelName;
        std::string _topic;
        std::vector<Client *> _members;
        std::vector<Client *> _operators;
    public:
        void addMember(Client *cli);
};

#endif