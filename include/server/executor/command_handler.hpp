#include <map>
#include <string>

class ACommand;
class Client;
class Server;
struct Message;

class command_handler {
 private:
  static std::map<std::string, ACommand*> _commandsMap;

 public:
  static void setupCommandsMap();
  static void handleCommands(Client& client, Message& msg);
};
