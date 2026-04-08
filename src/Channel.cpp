#include "Channel.hpp"

void Channel::addMember(Client* cli) {
    this->_members.push_back(cli);
}