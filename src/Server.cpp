#include "Server.hpp"
#include "CommandHandler.hpp"

extern volatile sig_atomic_t g_running;

Server::Server(int argc, char **argv) : _socket_fd(-1)
{
	parseArg(argc, argv);
}

uint16_t Server::parse_string_port(const char *port_str)
{
	char *endptr;
	long  port_num = std::strtol(port_str, &endptr, NUM_BASE);

	if (*endptr != '\0' || port_num <= 1024 || port_num > 65535)
	{
		throw std::invalid_argument("Invalid port number");
	}
	return htons((uint16_t) port_num);
}

void Server::parseArg(int argc, char **argv)
{
	if (argc != 3)
		throw std::invalid_argument("Please use format: ./irc <port> "
		                            "<password>");
	this->_port = std::atoi(argv[1]);
	this->_port_num = parse_string_port(argv[1]);
	this->_host_ip = inet_addr("127.0.0.1"); // sets the default address to localhost
	this->_password = std::string(argv[2]);
	if (this->_password.empty())
		throw std::out_of_range("Password must contain at least 1 letter");
}

/* ------------------ belongs to Hubert ------------------*/
void Server::executeMessage(Client &client, Message &msg, Server &server)
{
	switch (client.getState())
	{
	case CONNECTED:
	{
		std::cout << "inside CONNECTED: " << msg.command << std::endl;
		if (msg.command == "CAP" || msg.command == "PASS")
		{
			break;
		}
		std::cerr << "Login reguired: PASS" << std::endl;
		return;
	}
	case HANDSHAKE:
	{
		std::cout << "inside HANDSHAKE: " << msg.command << std::endl;
		if (msg.command == "NICK" || msg.command == "USER" || msg.command == "CAP")
		{
			break;
		}
		else
		{
			std::cerr << "ERR_NOTREGISTERED - logi --> HANDSHAKE" << std::endl;
			return;
		}
	}
	break;
	case REGISTERED:
	{
		std::cout << "inside REGISTERED: " << msg.command << std::endl;
		if (msg.command == "PASS" || msg.command == "USER" || msg.command == "NICK")
		{
			std::cerr << "ERR_ALREADYREGISTERED - log" << std::endl;
			return;
		}
		break;
	}
	}
	CommandHandler::handleCommand(client, msg, server);
}

void Server::run()
{
	init_socket(*this);
	int epoll_fd = init_epoll(*this);
	loop_epoll(epoll_fd, *this);
	close(epoll_fd);
}

Server::~Server()
{
	if (_socket_fd != -1)
	{
		close(_socket_fd);
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
	}
	_clients.clear();
}

uint32_t Server::getHostIp() const
{
	return this->_host_ip;
}
uint16_t Server::getPortNum() const
{
	return this->_port_num;
}
int Server::getSocketFd() const
{
	return this->_socket_fd;
}
void Server::setSocketFd(int fd)
{
	this->_socket_fd = fd;
}
std::string Server::getPassword()
{
	return this->_password;
}
std::map<int, Client> &Server::getClients()
{
	return this->_clients;
}

Channel *Server::getChannel(const std::string &chann)
{
	std::map<std::string, Channel>::iterator it = this->_channels.find(chann);
	if (it != this->_channels.end())
		return &it->second; // address of second map parameter - channel
	return NULL;
}

std::map<std::string, Channel> Server::getChannels()
{
	return this->_channels;
}

void Server::addChannel(Channel &ch, std::string &chName)
{
	ch.setChannelName(chName);
	this->_channels[chName] = ch;
}

/* listener */
int Server::create_socket()
{
	struct protoent *proto = getprotobyname("tcp");
	int              protocol_num = proto ? proto->p_proto : 0;

	int socket_fd = socket(AF_INET, SOCK_STREAM, protocol_num);
	if (socket_fd == -1)
	{
		throw std::runtime_error("Failed to create socket.");
	}

	int o = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(o)) == -1)
	{
		throw std::runtime_error("Failed to set socket SO_REUSEADDR.");
	}
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &o, sizeof(o)) == -1)
	{
		throw std::runtime_error("Failed to set socket SO_REUSEPORT.");
	}

	int flags = fcntl(socket_fd, F_GETFL);
	int status = fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
	if (status == -1)
	{
		throw std::runtime_error("Failed to set socket O_NONBLOCK");
	}
	return socket_fd;
}

void Server::bind_socket(int socket_fd, uint32_t ip_addr, uint16_t host)
{
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_addr;
	addr.sin_port = host;

	if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		throw std::runtime_error("Failed to bind socket.");
	}

	struct sockaddr_in bound_addr;
	socklen_t          bound_len = sizeof(bound_addr);
	if (getsockname(socket_fd, (struct sockaddr *) &bound_addr, &bound_len) != 0)
	{
		throw std::runtime_error("Failed to verify bind socket.");
	}
}

void Server::start_socket(int socket_fd)
{
	if (listen(socket_fd, SOMAXCONN) == -1)
	{
		throw std::runtime_error("Failed to listen on socket.");
	}
}

void Server::init_socket(Server &server)
{
	int socket_fd = create_socket();
	bind_socket(socket_fd, server.getHostIp(), server.getPortNum());
	start_socket(socket_fd);
	server.setSocketFd(socket_fd);
}

/* listener */
/* multiplexer */

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
					if (res == DROP_CONNECTION)
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
/* multiplexer */

/* request_handler */
bool Server::process_message(Client &client)
{
	size_t end_pos = client.getRequestBuffer().find(READ_END);
	if (end_pos != std::string::npos)
	{
		std::string messageLine = client.getRequestBuffer().substr(0, end_pos);
		client.getRequestBuffer().erase(0, end_pos + READ_END.size());
		Message msg;
		if (!messageLine.empty())
		{
			Parser::parseToStruct(messageLine, msg);
			this->executeMessage(client, msg, *this);
		}
		client.setStatus(READY_TO_RESPOND);
		return true;
	}
	return false;
}

HandleResult Server::read_chunk(Client &client)
{
	char recv_buffer[RECV_SIZE];
	ssize_t bytes = recv(client.getFd(), recv_buffer, sizeof(recv_buffer) - 1, 0);

	if (bytes <= 0)
		return DROP_CONNECTION;
	client.getRequestBuffer().append(recv_buffer, bytes);
	return KEEP_CONNECTION;
}

void Server::schedule_epollout(int epoll_fd, Client &client)
{
	struct epoll_event event = {};
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = client.getFd();
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.getFd(), &event);
}

void Server::schedule_epollin(int epoll_fd, Client &client)
{
	struct epoll_event event = {};
	event.events = EPOLLIN;
	event.data.fd = client.getFd();
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.getFd(), &event);
}

HandleResult Server::process_request(int epoll_fd, uint32_t events, Client &client)
{
	if (events & EPOLLIN && (!(events & EPOLLOUT)))
	{
		if (read_chunk(client) == DROP_CONNECTION)
		{
			return DROP_CONNECTION;
		}
	}

	while (1)
	{
		switch (client.getStatus())
		{
		case READING:
		{
			if (!process_message(client))
			{
				if (events & EPOLLOUT)
					schedule_epollin(epoll_fd, client);
				return KEEP_CONNECTION;
			}
		}
		// fall through
		case READY_TO_RESPOND:
		{
			if (events & EPOLLOUT)
			{
				if (respond(client) == DROP_CONNECTION)
					return DROP_CONNECTION;
				if (client.getResponseBuffer().empty())
					client.reset();
				else
					return KEEP_CONNECTION;
			}
			else
			{
				return KEEP_CONNECTION;
			}
		}
		}
	}
}
/* request_handler */

/* response stuff */
HandleResult Server::respond(Client &client)
{
	if (client.getResponseBuffer().empty())
	{
		return KEEP_CONNECTION;
	}

	const std::string &buf = client.getResponseBuffer();
	// in loop?
	ssize_t sent = send(client.getFd(), buf.c_str(), buf.size(), 0);

	if (sent <= 0)
	{
		return DROP_CONNECTION;
	}

	client.getResponseBuffer().erase(0, sent);
	return KEEP_CONNECTION;
}
/* response stuff */
