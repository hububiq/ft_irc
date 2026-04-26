#include "command_handler.hpp"  // deprecate the map in the future
#include "server_runner.hpp"

volatile sig_atomic_t g_running = 1;

void sig_handler(int signum) {
  (void)signum;
  g_running = 0;
}

int main(int argc, char **argv) {
  signal(SIGINT, sig_handler);
  signal(SIGQUIT, sig_handler);
  signal(SIGTERM, sig_handler);

  try {
    command_handler::setupCommandsMap();
    runner::setup(argc, argv);
    runner::run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}