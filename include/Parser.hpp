#ifndef PARSER_HPP
# define PARSER_HPP
# include "Message.hpp"
# include <stdexcept>
# include <string>

namespace Parser 
{
    void    parse(const std::string& rawMessage, Message& msg);
    void    extractParams(std::string line, Message& msg);
};

#endif