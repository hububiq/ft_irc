#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

enum ReplyCode 
{
    // --- Connection & Registration (001-099) ---
    RPL_WELCOME = 1,
    RPL_YOURHOST = 2, //
    RPL_CREATED = 3, //
    RPL_MYINFO = 4, //

    // --- Command replies 
    RPL_INVITING = 341, //
    RPL_TOPIC = 332,
    RPL_NOTOPIC = 331,
    
    // --- Command & Syntax Errors (400-439) ---
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404, //
    ERR_TOOMANYCHANNELS = 405,
    ERR_NORECIPIENT = 411, //
    ERR_NOTEXTTOSEND = 412, //
    ERR_UNKNOWNCOMMAND = 421, //
    ERR_NONICKNAMEGIVEN = 431, //
    ERR_ERRONEUSNICKNAME = 432, //
    ERR_NICKNAMEINUSE = 433, //

    // --- Channel & User Errors (440-459) ---
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443, //
    ERR_NOTREGISTERED = 451, //

    // --- Authentication & Access Errors (460-489) ---
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTERED = 462, //
    ERR_PASSWDMISMATCH = 464, //
    ERR_CHANNELISFULL = 471, //
    ERR_UNKNOWNMODE = 472, //
    ERR_INVITEONLYCHAN = 473, //
    ERR_BANNEDFROMCHAN = 474, //
    ERR_BADCHANNELKEY = 475, //
    ERR_NOPRIVILEGES = 481, //
    ERR_CHANOPRIVSNEEDED = 482
    // ---
};

namespace Replies {
    std::string getReply(ReplyCode code, const std::string& userNick, 
                     const std::string& arg1, const std::string& arg2);
}

#endif
