#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

#include <string>
#include <vector>

#include "Message.hpp"

namespace message_parser {

void deserialize(const std::string& rawLine, Message& outMessage);


} 

#endif