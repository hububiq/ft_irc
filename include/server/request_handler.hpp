#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <stdint.h>

#include "Client.hpp"
#include "HandlerStatus.hpp"
#include "Server.hpp"
#include "executor.hpp"
#include "epoll_state_manager.hpp"
#include "message_parser.hpp"

namespace request_handler {
HandleResult process_request(uint32_t events, Client &client);
}  // namespace request_handler

#endif
