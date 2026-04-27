#ifndef EPOLL_STATE_MANAGER_HPP
#define EPOLL_STATE_MANAGER_HPP

#include "Client.hpp"
#include "ServerDao.hpp"

namespace state_manager {
void schedule_epollin(Client &client);
void schedule_send();
}  // namespace state_manager

#endif
