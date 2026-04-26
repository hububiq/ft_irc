#ifndef RUNNER_HPP
#define RUNNER_HPP

#include "Server.hpp"
#include "listener.hpp"
#include "multiplexer.hpp"
#include "epoll_initializer.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace runner {
void setupCommandsMap();
void setup(int argc, char **argv);
void run();
}  // namespace runner

#endif
