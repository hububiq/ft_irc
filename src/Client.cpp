#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _status(READING) {}

int Client::getFd() const { return this->_fd; }

std::string& Client::getRequestBuffer() { return this->_request_buffer; }

std::string& Client::getResponseBuffer() { return this->_response_buffer; }

ClientStatus Client::getStatus() const { return this->_status; }

std::string& Client::getNickname() { return this->_nickname; }

std::string& Client::getRealName() { return this->_realname; }

bool Client::getRegisterInfo() const { return this->_isRegistered; }

bool Client::getAuthInfo() const { return this->_isAuthent; }

ClientState Client::getState() const { return this->_state; }

void Client::setRegister() { this->_isRegistered = true; }

void Client::setAuth() { this->_isAuthent = true; }

void Client::setState(ClientState state) { this->_state = state; }

void Client::setStatus(ClientStatus status) { this->_status = status; }

void Client::setHostname(std::string& hostname) { this->_hostname = hostname; }

void Client::setNickname(std::string nick) { this->_nickname = nick; }

void Client::setRealName(std::string rname) { this->_realname = rname; }

void Client::reset() {
  this->_status = READING;
  // this->_buffer could be cleared if needed, but since we parsed lines we
  // don't clear right now
}