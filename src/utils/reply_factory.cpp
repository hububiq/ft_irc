#include "reply_factory.hpp"

std::string reply_factory::getReply(ReplyCode code, const std::string &userNick,
                                    const std::string &arg1,
                                    const std::string &arg2) {
  std::string serverName = "localhost";
  std::string prefix = ":" + serverName + " ";

  switch (code) {
    // ==========================================
    // SUCCESSFUL REGISTRATION REPLIES
    // ==========================================
    case RPL_WELCOME:
      return prefix + "001 " + userNick +
             " :Welcome to the Internet Relay Network " + userNick + "!" +
             arg1 + "@" + arg2 + "\r\n";
    case RPL_TOPIC:
      return prefix + "332 " + userNick + " " + arg1 + " " + arg2 + "\r\n";
    case RPL_NOTOPIC:
      return prefix + "331 " + userNick + " " + arg1 + " " +
             ":No topic is set!\r\n";

    // ==========================================
    // COMMAND RENSPONSES
    // ==========================================
    case RPL_INVITING:
      return prefix + "341 " + userNick + " " + arg1 + " " + arg2 + "\r\n";

    case RPL_CHANNELMODEIS:
      return prefix + "324 " + userNick + " " + arg1 + " " + arg2 + "\r\n";

    // ==========================================
    // SYNTAX & COMMAND ERRORS
    // ==========================================
    case ERR_NOSUCHNICK:
      return prefix + "401 " + userNick + " " + arg1 +
             " :No such nick/channel\r\n";

    case ERR_NOSUCHCHANNEL:
      return prefix + "403 " + userNick + " " + arg1 + " :No such channel\r\n";

    case ERR_NORECIPIENT:
      return prefix + "411 " + userNick + " :No recipient given (" + arg1 +
             ")\r\n";

    case ERR_NOTEXTTOSEND:
      return prefix + "412 " + userNick + " :No text to send\r\n";

    case ERR_UNKNOWNCOMMAND:
      return prefix + "421 " + userNick + " " + arg1 + " :Unknown command\r\n";

    // ==========================================
    // NICKNAME ERRORS
    // ==========================================
    case ERR_NONICKNAMEGIVEN:
      return prefix + "431 " + userNick + " :No nickname given\r\n";
    case ERR_ERRONEUSNICKNAME:
      return prefix + "432 " + userNick + " " + arg1 +
             " :Erroneous nickname\r\n";

    case ERR_NICKNAMEINUSE:
      return prefix + "433 " + userNick + " " + arg1 +
             " :Nickname is already in use\r\n";

    // ==========================================
    // CHANNEL PRESENCE ERRORS
    // ==========================================
    case ERR_USERNOTINCHANNEL:
      return prefix + "441 " + userNick + " " + arg1 +
             " :They aren't on that channel\r\n";

    case ERR_NOTONCHANNEL:
      return prefix + "442 " + userNick + " " + arg1 + " " + arg2 +
             " :You're not on that channel\r\n";

    case ERR_USERONCHANNEL:
      return prefix + "443 " + userNick + " " + arg1 + " " + arg2 +
             " :is already on channel\r\n";

    // ==========================================
    // REGISTRATION & AUTHENTICATION ERRORS
    // ==========================================
    case ERR_NOTREGISTERED:
      return prefix + "451 " + userNick + " :You have not registered\r\n";

    case ERR_NEEDMOREPARAMS:
      return prefix + "461 " + userNick + " " + arg1 +
             " :Not enough parameters\r\n";

    case ERR_ALREADYREGISTERED:
      return prefix + "462 " + userNick +
             " :Unauthorized command (already registered)\r\n";

    case ERR_PASSWDMISMATCH:
      return prefix + "464 " + userNick + " :Password incorrect\r\n";

    // ==========================================
    // CHANNEL MODE & PRIVILEGE ERRORS
    // ==========================================
    case ERR_CHANNELISFULL:
      return prefix + "471 " + userNick + " " + arg1 +
             " :Cannot join channel (+l)\r\n";

    case ERR_UNKNOWNMODE:
      return prefix + "472 " + userNick + " " + arg1 +
             " :is unknown mode char to me\r\n";

    case ERR_KEYSET:
      return prefix + "467 " + userNick + " " + arg1 +
             " :Channel key already set\r\n";

    case ERR_INVITEONLYCHAN:
      return prefix + "473 " + userNick + " " + arg1 +
             " :Cannot join channel (+i)\r\n";

    case ERR_BADCHANNELKEY:
      return prefix + "475 " + userNick + " " + arg1 +
             " :Cannot join channel (+k)\r\n";

    case ERR_CHANOPRIVSNEEDED:
      return prefix + "482 " + userNick + " " + arg1 +
             " :You're not channel operator\r\n";

    default:
      return prefix + "500 " + userNick + " :Unknown Error\r\n";
  }
}
