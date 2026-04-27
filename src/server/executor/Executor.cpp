#include "Executor.hpp"

bool Executor::process_connected(Client &client, Message &msg)
{
	if (msg.command == CMD_CAP || msg.command == CMD_PASS)
	{
		return true;
	}
	std::string reply = reply_factory::getReply(ERR_NOTREGISTERED, "*", "", "");
	client.write_msg(reply);
	return false;
}

bool Executor::process_handshake(Client &client, Message &msg)
{
	if (msg.command == CMD_NICK || msg.command == CMD_USER || msg.command == CMD_CAP)
	{
		return true;
	}
	std::string reply = reply_factory::getReply(ERR_NOTREGISTERED, "*", "", "");
	client.write_msg(reply);
	return false;
}

bool Executor::process_registered(Client &client, Message &msg)
{
	if (msg.command == CMD_PASS || msg.command == CMD_USER || msg.command == CMD_NICK)
	{
		std::string nick = client.getNickname();
		std::string reply = reply_factory::getReply(ERR_ALREADYREGISTERED, nick, "", "");
		client.write_msg(reply);
		return false;
	}
	return true;
}

bool Executor::validateClientState(Client &client, Message &msg)
{
	ClientState state = client.getState();

	switch (state)
	{
	case CONNECTED:
		return (process_connected(client, msg));
	case HANDSHAKE:
		return (process_handshake(client, msg));
	case REGISTERED:
		return (process_registered(client, msg));
	}
	return true;
}

Executor::Executor(CommandHandler *commandHandler)
    : m_commandHandler(commandHandler)
{
}

Executor::~Executor()
{
	if (m_commandHandler)
		delete m_commandHandler;
}

void Executor::executeMessage(Client &client, Message &msg)
{
	if (!validateClientState(client, msg))
	{
		return;
	}
	m_commandHandler->handleCommand(client, msg);
}
