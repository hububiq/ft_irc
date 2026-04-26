void Server::register_socket(int epoll_fd, int socket_fd)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = socket_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
	{
		throw std::runtime_error("Failed to add socket to epoll.");
	}
}

int Server::init_epoll(Server &server)
{
	int epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		throw std::runtime_error("Failed to create epoll file descriptor.");
	}
	register_socket(epoll_fd, server.getSocketFd());
	return epoll_fd;
}

void Server::loop_epoll(int epoll_fd, Server &server)
{
	struct epoll_event events[LIMIT];
	while (g_running)
	{
		int num_ready = epoll_wait(epoll_fd, events, LIMIT, TIMEOUT);
		for (int i = 0; i < num_ready; i++)
		{
			int event_fd = events[i].data.fd;
			if (event_fd == server.getSocketFd())
			{
				process_connect(epoll_fd, event_fd, server._clients);
			}
			else
			{
				std::map<int, Client>::iterator it = server._clients.find(event_fd);
				if (it != server._clients.end())
				{
					HandleResult res = process_request(epoll_fd, events[i].events, it->second);
					if (res == DROP_CONNECTION || it->second.getStatus() == DISCONNECTING)
					{
						std::cout << "Client disconnected" << std::endl;
						server._clients.erase(it);
						close(event_fd);
					}
				}
			}
		}

		for (std::map<int, Client>::iterator it = server._clients.begin();
		     it != server._clients.end(); ++it)
		{
			if (it->second.getStatus() == READY_TO_RESPOND)
			{
				server.schedule_epollout(epoll_fd, it->second);
			}
		}
	}
}

void Server::register_client(int client_fd, std::string &hostname, std::map<int, Client> &clients)
{
	Client c(client_fd);
	c.setHostname(hostname);
	c.setState(CONNECTED);
	clients.insert(std::make_pair(client_fd, c));
	std::cout << "Client connected" << std::endl;
}

void Server::process_connect(int epoll_fd, int socket_fd, std::map<int, Client> &clients)
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
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
		register_client(client_fd, hostname, clients);
	}
}