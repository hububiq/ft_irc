#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <stdint.h>
#include "Server.hpp"
#include "conn_handler.hpp"
#include "request_handler.hpp"
#include "epoll_state_manager.hpp"

namespace multiplexer {
void loop_epoll();
}  // namespace multiplexer

#endif
