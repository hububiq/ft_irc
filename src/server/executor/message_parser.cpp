#include "message_parser.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>

namespace {
std::string normalizeCommand(std::string command);
void extractParams(std::string& input, Message& msg);

void extractPrefix(std::string& input, Message& msg) {
  if (input[0] != ':') return;

  size_t spacePos = input.find(' ');
  if (spacePos == std::string::npos) {
    throw std::invalid_argument(
        "Malformed IRC message: Prefix provided without command.");
  }

  msg.prefix = input.substr(1, spacePos - 1);

  input.erase(0, spacePos + 1);
}

void trimLeadingWhitespace(std::string& input) {
  size_t firstContentChar = input.find_first_not_of(" \t");
  if (firstContentChar == std::string::npos) {
    throw std::invalid_argument("Message contains no command.");
  }
  input.erase(0, firstContentChar);
}

void extractCommand(std::string& input, Message& msg) {
  size_t spacePos = input.find(' ');
  std::string rawCommand;

  if (spacePos == std::string::npos) {
    rawCommand = input;
    input.clear();
  } else {
    rawCommand = input.substr(0, spacePos);
    input.erase(0, spacePos + 1);
  }

  msg.command = normalizeCommand(rawCommand);
}

std::string normalizeCommand(std::string command) {
  std::transform(command.begin(), command.end(), command.begin(),
                 static_cast<int (*)(int)>(std::toupper));
  return command;
}

void extractParams(std::string& input, Message& msg) {
  while (!input.empty()) {
    size_t first = input.find_first_not_of(' ');
    if (first == std::string::npos) break;
    input.erase(0, first);

    if (input.empty()) break;
    if (input[0] == ':') {
      msg.params.push_back(input.substr(1));
      input.clear();
      break;
    }

    size_t spacePos = input.find(' ');
    if (spacePos == std::string::npos) {
      msg.params.push_back(input);
      input.clear();
      break;
    }

    msg.params.push_back(input.substr(0, spacePos));
    input.erase(0, spacePos + 1);
  }
}

}  // namespace

namespace message_parser {

void deserialize(const std::string& rawLine, Message& outMessage) {
  if (rawLine.empty()) {
    throw std::invalid_argument("Cannot parse an empty message.");
  }

  std::string input = rawLine;

  extractPrefix(input, outMessage);
  trimLeadingWhitespace(input);
  extractCommand(input, outMessage);

  if (!input.empty()) {
    extractParams(input, outMessage);
  }
}

}  // namespace message_parser