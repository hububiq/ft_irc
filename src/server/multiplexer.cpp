#include "multiplexer.hpp"

extern volatile sig_atomic_t g_running;
extern Server *g_server;

void multiplexer::loop_epoll() {
  struct epoll_event events[LIMIT];
  while (g_running) {
    int num_ready = epoll_wait(g_server->getEpollFd(), events, LIMIT, TIMEOUT);
    for (int i = 0; i < num_ready; i++) {
      int event_fd = events[i].data.fd;
      if (event_fd == g_server->getSocketFd()) {
        conn_handler::process_connect(event_fd);
      } else {
        std::map<int, Client>::iterator it =
            g_server->getClients().find(event_fd);
        if (it != g_server->getClients().end()) {
          HandleResult res =
              request_handler::process_request(events[i].events, it->second);
          if (res == DROP_CONNECTION) {
            std::cout << "Client disconnected" << std::endl;
            g_server->getClients().erase(it);
            close(event_fd);
          }
        }
      }
    }
    state_manager::schedule_send();
  }
}