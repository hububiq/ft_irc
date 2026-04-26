#include <map>
#include <string>

class ACommand;
class Client;
class Server;
struct Message;

class CommandHandler {
 private:
  static std::map<std::string, ACommand*> _commandsMap;

 public:
  static void initCommands();
  static void handleCommands(Client& client, Message& msg);
};
