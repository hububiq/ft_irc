#ifndef CLIENTSTATUS_HPP
#define CLIENTSTATUS_HPP

enum ClientStatus
{
	READING,
	READY_TO_RESPOND
};

enum ClientState
{
	CONNECTED,
	HANDSHAKE,
	REGISTERED
};

enum HandleResult
{
	KEEP_CONNECTION,
	DROP_CONNECTION
};

const std::string READ_END = "\r\n";

#endif
