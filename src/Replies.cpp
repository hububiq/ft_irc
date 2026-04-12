#include "../include/Replies.hpp"

std::string Replies::getReply(ReplyCode code, const std::string& userNick, 
                                   const std::string& arg1, const std::string& arg2) 
{
    
    // RFC format: :<server> <code> <nick> <args> :<msg>\r\n
    std::string serverName = "localhost"; 
    std::string prefix = ":" + serverName + " ";

    switch (code) {
        
        // --- SUCCESSFUL REPLIES ---
        case RPL_WELCOME:
            return prefix + "001 " + userNick + " :Welcome to the Internet Relay Network " + 
                   userNick + "!" + arg1 + "@" + arg2 + "\r\n";
        case RPL_INVITING:
            return prefix + "341 " + userNick + " " + arg1 + " " + arg2;

        // --- ERROR REPLIES ---
        case ERR_NOSUCHNICK:
            // arg1 = the nickname that doesn't exist
            return prefix + "401 " + userNick + " " + arg1 +  " :No such nick/channel\r\n";

        case ERR_NOSUCHCHANNEL:
            // arg1 = the channel name
            return prefix + "403 " + userNick + " " + arg1 +  " :No such channel\r\n";

        case ERR_TOOMANYCHANNELS:
            return prefix + "405 " + userNick + " " + arg1 + " ::You have joined too many channels\r\n";

        case ERR_USERNOTINCHANNEL:
            return prefix + "441 " + userNick + " " + arg1 + " :They aren't on that channel\r\n";

        case ERR_NOTONCHANNEL:
            return prefix + "442 " + arg1 + " :You're not on that channel\r\n";

        case ERR_NEEDMOREPARAMS:
            // arg1 = the command they typed (e.g., "KICK")
            return prefix + "461 " + userNick + " " + arg1 + " :Not enough parameters\r\n";

        case ERR_CHANOPRIVSNEEDED:
            // arg1 = the channel name
            return prefix + "482 " + userNick + " " + arg1 + " :You're not channel operator\r\n";
        
        case ERR_USERONCHANNEL:
            // argv 1 = invited user
            // arg2 = the channel name
            return prefix + "443 " + userNick + " " + arg1 + " " + arg2 + " :is already on channel\r\n";

        default:
            return prefix + "500 " + userNick + " :Unknown Error\r\n";
    }
}
