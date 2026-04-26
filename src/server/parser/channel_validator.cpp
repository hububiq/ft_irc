bool isValidChannelName(std::string& channName) {
  if (channName.size() < 2 || channName[0] != '#' || channName.size() > 200)
    return false;
  for (size_t i = 1; i < channName.size(); i++) {
    char c = channName[i];
    if (c == ' ' || c == '\0' || c == '\r' || c == '\n' || c == ',')
      return false;
  }
  return true;
}
