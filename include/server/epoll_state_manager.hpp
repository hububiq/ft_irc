#ifndef EPOLL_STATE_MANAGER_HPP
#define EPOLL_STATE_MANAGER_HPP

#include "Server.hpp"
#include "Client.hpp"

namespace state_manager {
void schedule_epollin(Client &client);
void schedule_send();
}  // namespace epoll_state_manager

#endif
