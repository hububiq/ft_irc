#ifndef LISTENER_HPP
#define LISTENER_HPP

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


namespace listener {
int init_socket(uint32_t host_ip, uint16_t port_num);
}  // namespace listener

#endif
