#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

#include <string>
#include <vector>

#include "Message.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>

class MessageParser {
public:
  void deserialize(const std::string& rawLine, Message& outMessage);

private:
  std::string normalizeCommand(std::string command);
  void extractParams(std::string& input, Message& msg);
  void extractPrefix(std::string& input, Message& msg);
  void trimLeadingWhitespace(std::string& input);
  void extractCommand(std::string& input, Message& msg);
};

#endif