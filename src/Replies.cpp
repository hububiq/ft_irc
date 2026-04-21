#include "../include/Replies.hpp"

std::string Replies::getReply(ReplyCode code, const std::string& userNick, 
                                   const std::string& arg1, const std::string& arg2) 
{
    
    // RFC format: :<server> <code> <nick> <args> :<msg>\r\n
    std::string serverName = "localhost"; 
    std::string prefix = ":" + serverName + " ";

    switch (code)
    {
        // ==========================================
        // SUCCESSFUL REGISTRATION REPLIES
        // ==========================================
        case RPL_WELCOME:
            return prefix + "001 " + userNick + " :Welcome to the Internet Relay Network " + 
                   userNick + "!" + arg1 + "@" + arg2 + "\r\n";
        
        // case RPL_YOURHOST:
        //     // arg1: server version
        //     return prefix + "002 " + userNick + " :Your host is " + serverName + ", running version " + arg1 + "\r\n";
            
        // case RPL_CREATED:
        //     // arg1: creation date
        //     return prefix + "003 " + userNick + " :This server was created " + arg1 + "\r\n";

        // case RPL_MYINFO:
            // arg1: version, arg2: user modes, arg3: channel modes
            //:server 004 <nick> <servername> <version> <available umodes> <available cmodes> [<cmodes with param>]
            // ??? -> return prefix + "004 " + userNick + " " + serverName + " " + arg1 + " " + arg2 + " " + arg3 + "\r\n";

        case RPL_TOPIC:
            //shows the topic upon command or upon joining. arg1 is channel, arg2 the topic
            return prefix + "332 " + userNick + " " + arg1 + " " + arg2 + "\r\n";
        case RPL_NOTOPIC:
            //shows info that there is no topic in case of input TOPIC with just channel as msg.params[0]
            //arg1 is channel again, there is no arg2 but hardcoded message \/
            return prefix + "331 " + userNick + " " + arg1 + " " + ":No topic is set!\r\n";

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
            // arg1 = the nickname that doesn't exist
            return prefix + "401 " + userNick + " " + arg1 +  " :No such nick/channel\r\n";

        case ERR_NOSUCHCHANNEL:
            // arg1 = the channel name
            return prefix + "403 " + userNick + " " + arg1 +  " :No such channel\r\n";
        
        // case ERR_CANNOTSENDTOCHAN:
        //     // arg1: channel name
        //     return prefix + "404 " + userNick + " " + arg1 + " :Cannot send to channel\r\n";

        // case ERR_TOOMANYCHANNELS:
        //     return prefix + "405 " + userNick + " " + arg1 + " ::You have joined too many channels\r\n";
        
        case ERR_NORECIPIENT:
            // arg1: command name
            return prefix + "411 " + userNick + " :No recipient given (" + arg1 + ")\r\n";

        case ERR_NOTEXTTOSEND:
            return prefix + "412 " + userNick + " :No text to send\r\n";

        case ERR_UNKNOWNCOMMAND:
            // arg1: command name
            return prefix + "421 " + userNick + " " + arg1 + " :Unknown command\r\n";

        // ==========================================
        // NICKNAME ERRORS
        // ==========================================
        // case ERR_NONICKNAMEGIVEN:
        //     return prefix + "431 :No nickname given\r\n";

        case ERR_ERRONEUSNICKNAME:
            // arg1: bad nickname
            return prefix + "432 " + userNick + " " + arg1 + " :Erroneous nickname\r\n";

        case ERR_NICKNAMEINUSE:
            // arg1: taken nickname
            return prefix + "433 " + userNick + " " + arg1 + " :Nickname is already in use\r\n";
        // ==========================================
        // CHANNEL PRESENCE ERRORS
        // ==========================================
        case ERR_USERNOTINCHANNEL:
            return prefix + "441 " + userNick + " " + arg1 + " :They aren't on that channel\r\n";

        case ERR_NOTONCHANNEL:
            return prefix + "442 " + userNick + " " + arg1 + " " + arg2 + " :You're not on that channel\r\n";
            
        case ERR_USERONCHANNEL:
            // argv 1 = invited user
            // arg2 = the channel name
            return prefix + "443 " + userNick + " " + arg1 + " " + arg2 + " :is already on channel\r\n";

        // ==========================================
        // REGISTRATION & AUTHENTICATION ERRORS
        // ==========================================
        case ERR_NOTREGISTERED:
            return prefix + "451 " + userNick + " :You have not registered\r\n";
        
        case ERR_NEEDMOREPARAMS:
            // arg1 = the command they typed (e.g., "KICK")
            return prefix + "461 " + userNick + " " + arg1 + " :Not enough parameters\r\n";
        
        case ERR_ALREADYREGISTERED:
            return prefix + "462 " + userNick + " :Unauthorized command (already registered)\r\n";

        case ERR_PASSWDMISMATCH:
            return prefix + "464 " + userNick + " :Password incorrect\r\n";

        // ==========================================
        // CHANNEL MODE & PRIVILEGE ERRORS
        // ==========================================
        case ERR_CHANNELISFULL:
            // arg1: channel name
            return prefix + "471 " + userNick + " " + arg1 + " :Cannot join channel (+l)\r\n";

        case ERR_UNKNOWNMODE:
            // arg1: mode char
            return prefix + "472 " + userNick + " " + arg1 + " :is unknown mode char to me\r\n";

        case ERR_KEYSET:
            return prefix + "467 " + userNick + " " + arg1 + " :Channel key already set\r\n";

        case ERR_INVITEONLYCHAN:
            // arg1: channel name
            return prefix + "473 " + userNick + " " + arg1 + " :Cannot join channel (+i)\r\n";

        // case ERR_BANNEDFROMCHAN:
        //     // arg1: channel name
        //     return prefix + "474 " + userNick + " " + arg1 + " :Cannot join channel (+b)\r\n";

        case ERR_BADCHANNELKEY:
            // arg1: channel name
            return prefix + "475 " + userNick + " " + arg1 + " :Cannot join channel (+k)\r\n";

        // case ERR_NOPRIVILEGES:
        //     return prefix + "481 " + userNick + " :Permission Denied- You're not an IRC operator\r\n";
        
        case ERR_CHANOPRIVSNEEDED:
            // arg1 = the channel name
            return prefix + "482 " + userNick + " " + arg1 + " :You're not channel operator\r\n";
  
        default:
            return prefix + "500 " + userNick + " :Unknown Error\r\n";
    }
}
