#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <arpa/inet.h>

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include "Server.hpp"
#include "epoll_initializer.hpp"
#include "listener.hpp"
#include "multiplexer.hpp"

namespace runner {
void setupCommandsMap();
void setup(int argc, char **argv);
void run();
}  // namespace runner

#endif
