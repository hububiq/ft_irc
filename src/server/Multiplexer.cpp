#include "Multiplexer.hpp"

extern volatile sig_atomic_t g_running;
Multiplexer::Multiplexer(ServerDao *server, ConnHandler *connHandler, RequestHandler *requestHandler, EpollStateManager *stateManager)
    : m_server(server), m_connHandler(connHandler), m_requestHandler(requestHandler), m_stateManager(stateManager)
{
}

Multiplexer::~Multiplexer()
{
	if (m_connHandler)
		delete m_connHandler;
	if (m_requestHandler)
		delete m_requestHandler;
	if (m_stateManager)
		delete m_stateManager;
}

void Multiplexer::loop_epoll()
{
	struct epoll_event events[LIMIT];
	while (g_running)
	{
		int num_ready = epoll_wait(m_server->getEpollFd(), events, LIMIT, TIMEOUT);
		for (int i = 0; i < num_ready; i++)
		{
			int event_fd = events[i].data.fd;
			if (event_fd == m_server->getSocketFd())
			{
				m_connHandler->process_connect(event_fd);
			}
			else
			{
				std::map<int, Client>::iterator it = m_server->getClients().find(event_fd);
				if (it != m_server->getClients().end())
				{
					HandleResult res =
					    m_requestHandler->process_request(events[i].events, it->second);
					if (res == DROP_CONNECTION)
					{
						std::cout << "Client disconnected" << std::endl;
						m_server->getClients().erase(it);
						close(event_fd);
					}
				}
			}
		}
		m_stateManager->schedule_send();
	}
}