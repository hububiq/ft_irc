#include <iostream>

#include "Server.hpp"
#include "CommandHandler.hpp"

int main(int argc, char** argv) {
  try {
    CommandHandler::initCommands();
    Server server(argc, argv);
    server.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}