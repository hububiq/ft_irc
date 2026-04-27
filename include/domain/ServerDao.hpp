#ifndef SERVER_HPP
#define SERVER_HPP
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cctype>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "EpollConfig.hpp"
#include "HandlerStatus.hpp"
#include "Message.hpp"

class ServerDao
{
  private:
	int                            _socket_fd;
	int                            _epoll_fd;
	int                            _port;
	uint32_t                       _host_ip;
	uint16_t                       _port_num;
	std::string                    _password;
	std::map<int, Client>          _clients;
	std::map<std::string, Channel> _channels;

  public:
	ServerDao(int port, uint16_t port_num, uint32_t host_ip, const std::string &password, int socket_fd, int epoll_fd);
	~ServerDao();

	std::map<int, Client> &getClients();
	bool                   checkIfClientExistsByNickname(std::string &nickName);
	int                    getSocketFd() const;
	int                    getEpollFd() const;
	int                    getPort() const;
	uint32_t               getHostIp() const;
	uint16_t               getPortNum() const;
	const std::string     &getPassword() const;

	Channel                        *getChannel(const std::string &chann);
	std::map<std::string, Channel> &getChannels();
	void addChannel(Channel &ch, std::string &chName);
	bool isClientAdmin(Client &client, const std::string &chName);
	bool isInviteeInChannel(const Message &msg, const std::string &channelName);
	bool isUserInChannel(Client &client, const std::string &channelName);
};

#endif
