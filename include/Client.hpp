#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>

#include "ClientStatus.hpp"

class Client 
{
private:
	int _fd;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _hostname;
	ClientStatus _status;
	ClientState _state;
	bool _isRegistered;
	bool _isAuthent;

	std::string _request_buffer;
	std::string _response_buffer;

public:
	Client(int fd);

	int getFd() const;
	bool getRegisterInfo() const;
	bool getAuthInfo() const;
	ClientState getState() const;
	const std::string& getNickname();
	const std::string& getUsername();
	const std::string& getRealName();
	std::string& getRequestBuffer();
	std::string& getResponseBuffer();
	ClientStatus getStatus() const;
	std::string getHostname() const;
	void setStatus(ClientStatus status);
	void setRegister();
	void setAuth();
	void setState(ClientState state);
	void setHostname(std::string& hostname);
	void setNickname(std::string nick);
	void setUsername(std::string username);
	void setRealName(std::string rname);
	void reset();
	void write_msg(std::string& message);
};

#endif
