#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _status(READING) {}

int Client::getFd() const { return this->_fd; }

std::string& Client::getRequestBuffer() { return this->_request_buffer; }

std::string& Client::getResponseBuffer() { return this->_response_buffer; }

ClientStatus Client::getStatus() const { return this->_status; }

bool Client::getRegisterInfo() const { return this->_isRegistered; }

bool Client::getAuthInfo() const { return this->_isAuthent; }

ClientState Client::getState() const { return this->_state; }

void Client::setAuth() { this->_isAuthent = true; }

void Client::setState(ClientState state) { this->_state = state; }

void Client::setStatus(ClientStatus status) { this->_status = status; }

void Client::reset() {
  this->_status = READING;
  // this->_buffer could be cleared if needed, but since we parsed lines we
  // don't clear right now
}