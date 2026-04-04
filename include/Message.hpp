#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include <vector>
# include <string>

struct Message 
{
    std::string                 prefix;
    std::string                 command;
    std::vector<std::string>    params;
};

#endif