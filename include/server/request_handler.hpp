#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <stdint.h>

#include "Client.hpp"
#include "HandlerStatus.hpp"
#include "ServerDao.hpp"
#include "epoll_state_manager.hpp"
#include "executor.hpp"
#include "message_parser.hpp"

class Executor;

class RequestHandler {
public:
  RequestHandler(EpollStateManager *stateManager, Executor *executor, MessageParser *messageParser);
  ~RequestHandler();
  HandleResult process_request(uint32_t events, Client &client);

private:
  EpollStateManager *m_stateManager;
  Executor *m_executor;
  MessageParser *m_messageParser;

  bool process_message(Client &client);
  HandleResult read_chunk(Client &client);
  HandleResult respond(Client &client);
};

#endif
