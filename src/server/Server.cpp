#include "Server.hpp"
#include "CommandHandler.hpp"
#include "Replies.hpp" 

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

void Server::parseArg(int argc, char** argv) {
  if (argc != 3)
    throw std::invalid_argument("Please use format: ./irc <port> <password>");
  this->_port = std::atoi(argv[1]);
  this->_port_num = parse_string_port(argv[1]);
  this->_host_ip = inet_addr("127.0.0.1");
  this->_password = std::string(argv[2]);
  if (this->_password.empty())
    throw std::out_of_range("Password must contain at least 1 letter");
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

Channel* Server::getChannel(const std::string& chann) {
  std::map<std::string, Channel>::iterator it = this->_channels.find(chann);
  if (it != this->_channels.end())
    return &it->second;
  return NULL;
}

std::map<std::string, Channel>& Server::getChannels()
{
	return this->_channels;
}

void Server::addChannel(Channel &ch, std::string &chName)
{
	ch.setChannelName(chName);
	this->_channels[chName] = ch;
}