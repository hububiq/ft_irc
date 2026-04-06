#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>

#include "ClientStatus.hpp"

class Client {
 private:
  int _fd;
  std::string _nickname;
  std::string _username;
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
  std::string& getRequestBuffer();
  std::string& getResponseBuffer();
  ClientStatus getStatus() const;
  void setStatus(ClientStatus status);
  void setAuth();
  void setState(ClientState state);
  void reset();
};

#endif