bool isValidNickname(std::string& nick, Client& cli) {
  std::string specials = "-\\[]`^{}";
  if (nick.size() > 9 || !isalpha(nick[0])) {
    std::string reply = Replies::getReply(ERR_ERRONEUSNICKNAME, "*", nick, "");
    cli.write_msg(reply);
    return false;
  }
  for (size_t i = 1; i < nick.size(); i++) {
    if (!isalpha(nick[i]) && !isdigit(nick[i]) &&
        specials.find(nick[i]) == std::string::npos) {
      std::string reply =
          Replies::getReply(ERR_ERRONEUSNICKNAME, "*", nick, "");
      cli.write_msg(reply);
      return false;
    }
  }
  return true;
}