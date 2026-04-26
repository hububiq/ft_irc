#ifndef STATE_UTILS_HPP
#define STATE_UTILS_HPP

enum HandlerStatus { READING, READY_TO_RESPOND };

enum HandleResult { KEEP_CONNECTION, DROP_CONNECTION };

const std::string READ_END = "\r\n";

#endif
