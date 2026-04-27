#include "ConnHandler.hpp"

ConnHandler::ConnHandler(ServerDao *server) : m_server(server)
{
}

void ConnHandler::register_client(int client_fd, std::string &hostname)
{
	Client c(client_fd);
	c.setHostname(hostname);
	c.setState(CONNECTED);
	m_server->getClients().insert(std::make_pair(client_fd, c));
	std::cout << "Client connected" << std::endl;
}

void ConnHandler::process_connect(int socket_fd)
{
	struct sockaddr_in addr;
	socklen_t          len = sizeof(addr);

	int client_fd = accept(socket_fd, (struct sockaddr *) &addr, &len);
	if (client_fd >= 0)
	{
		std::string hostname = inet_ntoa(addr.sin_addr);
		int         flags = fcntl(client_fd, F_GETFL, 0);
		if (flags != -1)
		{
			fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
		}
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = client_fd;
		epoll_ctl(m_server->getEpollFd(), EPOLL_CTL_ADD, client_fd, &event);
		register_client(client_fd, hostname);
	}
}