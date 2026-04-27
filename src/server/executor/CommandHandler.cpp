#include "CommandHandler.hpp"

CommandHandler::CommandHandler(ServerDao *server, Validator *Validator, JoinGatekeeper *joinGatekeeper, ModeReporter *modeReporter)
    : m_validator(Validator), m_joinGatekeeper(joinGatekeeper), m_modeReporter(modeReporter)
{
	_commandsMap["PASS"] = new Pass(server);
	_commandsMap["NICK"] = new Nick(server, Validator);
	_commandsMap["USER"] = new User();
	_commandsMap["JOIN"] = new Join(server, Validator, joinGatekeeper);
	_commandsMap["PRIVMSG"] = new PrivMsg(server);
	_commandsMap["KICK"] = new Kick(server);
	_commandsMap["INVITE"] = new Invite(server, Validator);
	_commandsMap["PING"] = new Ping();
	_commandsMap["CAP"] = new Cap();
	_commandsMap["TOPIC"] = new Topic(server);
	_commandsMap["MODE"] = new Mode(server, modeReporter);
	_commandsMap["QUIT"] = new Quit(server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, ACommand *>::iterator it = _commandsMap.begin();
	     it != _commandsMap.end(); ++it)
	{
		delete it->second;
	}
	if (m_validator)
		delete m_validator;
	if (m_joinGatekeeper)
		delete m_joinGatekeeper;
	if (m_modeReporter)
		delete m_modeReporter;
}

void CommandHandler::handleCommand(Client &client, Message &msg)
{
	std::string cmdName = msg.command;

	std::map<std::string, ACommand *>::iterator it = _commandsMap.find(cmdName);
	if (it != _commandsMap.end())
		it->second->execute(client, msg);
	else
	{
		std::string reply = reply_factory::getReply(ERR_UNKNOWNCOMMAND, client.getNickname(), cmdName, "");
		client.write_msg(reply);
	}
}
