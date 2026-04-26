void extractParams(std::string line, Message& msg) {
  std::istringstream ss(line);
  std::string token;

  while (ss >> token) {
    if (token[0] == ':') {
      std::string rest;
      std::getline(ss, rest);
      msg.params.push_back(token.substr(0) + rest);
      break;
    } else
      msg.params.push_back(token);
  }
}

void parseToStruct(const std::string& rawMessage, Message& msg) {
  std::string line = rawMessage;
  if (line[0] == ':') {
    size_t prefixEnd = line.find(' ');
    if (prefixEnd == std::string::npos)
      throw std::invalid_argument("Error: Malformed message.");
    msg.prefix = line.substr(1, prefixEnd - 1);
    line.erase(0, prefixEnd + 1);
  }
  size_t firstChar = line.find_first_not_of(" \t");
  if (firstChar != std::string::npos)
    line.erase(0, firstChar);
  else
    throw std::invalid_argument("Error: Line was empty.");
  size_t commandEnd = line.find(' ');
  if (commandEnd == std::string::npos) {
    std::transform(line.begin(), line.end(), line.begin(), ::toupper);
    msg.command = line;
    line.clear();
  } else {
    msg.command = line.substr(0, commandEnd);
    std::transform(msg.command.begin(), msg.command.end(), msg.command.begin(),
                   ::toupper);
    line.erase(0, commandEnd + 1);
    extractParams(line, msg);
  }
}