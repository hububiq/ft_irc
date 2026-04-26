// #include "Client.hpp"
#include "Client.hpp"

#include <sys/socket.h>

#include <iostream>
#include <string>

Client::Client(int fd)
    : _fd(fd), _status(READING), _isRegistered(false), _isAuthent(false) {}

int Client::getFd() const { return this->_fd; }

std::string &Client::getRequestBuffer() { return this->_request_buffer; }

std::string &Client::getResponseBuffer() { return this->_response_buffer; }

HandlerStatus Client::getStatus() const { return this->_status; }

const std::string &Client::getNickname() { return this->_nickname; }

const std::string &Client::getUsername() { return this->_username; }

const std::string &Client::getRealName() { return this->_realname; }

bool Client::getRegisterInfo() const { return this->_isRegistered; }

bool Client::getAuthInfo() const { return this->_isAuthent; }

ClientState Client::getState() const { return this->_state; }

std::string Client::getHostname() const { return this->_hostname; }

void Client::setRegister() { this->_isRegistered = true; }

void Client::setAuth() { this->_isAuthent = true; }

void Client::setState(ClientState state) { this->_state = state; }

void Client::setStatus(HandlerStatus status) { this->_status = status; }

void Client::setHostname(std::string &hostname) { this->_hostname = hostname; }

void Client::setNickname(std::string nick) { this->_nickname = nick; }

void Client::setUsername(std::string usname) { this->_username = usname; }

void Client::setRealName(std::string rname) { this->_realname = rname; }

void Client::reset() {
  this->_status = READING;
  // this->_buffer could be cleared if needed, but since we parsed lines we
  // don't clear right now
}

void Client::write_msg(
    std::string &message)  // queue message and let state machine handle send
{
  this->_response_buffer += message;
  this->_status = READY_TO_RESPOND;
}