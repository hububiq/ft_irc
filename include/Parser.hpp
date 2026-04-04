#ifndef PARSER_HPP
#define PARSER_HPP
#include <stdexcept>
#include <string>

#include "Message.hpp"

namespace Parser {
void parse(const std::string& rawMessage, Message& msg);
void extractParams(std::string line, Message& msg);
};  // namespace Parser

#endif