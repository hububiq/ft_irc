#include "message_parser.hpp"

namespace {
void extractPrefix(std::string& input, Message& msg) {
  if (input[0] != ':') return;

  size_t spacePos = input.find(' ');
  if (spacePos == std::string::npos) {
    throw std::invalid_argument("Malformed IRC message: Prefix provided without command.");
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
  std::transform(command.begin(), command.end(), command.begin(), ::toupper);
  return command;
}

}

void deserialize(std::string rawLine, Message& outMessage) {
  if (rawLine.empty()) {
    throw std::invalid_argument("Cannot parse an empty message.");
  }

  extractPrefix(rawLine, outMessage);
  trimLeadingWhitespace(rawLine);
  extractCommand(rawLine, outMessage);

  if (!rawLine.empty()) {
    extractParams(rawLine, outMessage);
  }
}