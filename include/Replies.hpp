#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

enum ReplyCode 
{
    RPL_WELCOME = 1,
    
    // Error Replies
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_TOOMANYCHANNELS = 405,
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_NEEDMOREPARAMS = 461,
    ERR_CHANOPRIVSNEEDED = 482
    // ---
};

namespace Replies {
    std::string getReply(ReplyCode code, const std::string& userNick, 
                     const std::string& arg1, const std::string& arg2);
}

#endif
