#ifndef CLIENTSTATUS_HPP
#define CLIENTSTATUS_HPP

enum ClientStatus {
  READING,
  READY_TO_RESPOND
};

const std::string READ_END = "\r\n";

#endif
