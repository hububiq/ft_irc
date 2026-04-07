#ifndef PARSER_HPP
#define PARSER_HPP
#include <stdexcept>
#include <string>

#include "Message.hpp"
#include "Client.hpp"

namespace Parser {
void parseToStruct(const std::string& rawMessage, Message& msg);
void extractParams(std::string line, Message& msg);
bool isValidNickname(std::string& nick);
};

#endif