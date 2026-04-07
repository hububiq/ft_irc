#ifndef CLIENTSTATUS_HPP
#define CLIENTSTATUS_HPP

enum ClientStatus {
  READING,
  READY_TO_RESPOND
};
/*Szymon, you can merge these two enums if you need yours*/
enum ClientState
{
    CONNECTED,
    HANDSHAKE,
    REGISTERED
};

enum HandleResult { KEEP_CONNECTION, DROP_CONNECTION };

const std::string READ_END = "\r\n";

#endif
