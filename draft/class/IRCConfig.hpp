#ifndef IRC_CONFIG_HPP
# define IRC_CONFIG_HPP

#include <string>

class IRCConfig
{
    private:
        int _port;
        std::string _password;
    public:
        IRCConfig();
        IRCConfig(const IRCConfig &other);
        virtual ~IRCConfig();
        IRCConfig operator=(const IRCConfig &rhs);
        
        IRCConfig(int port, std::string password);
};

#endif // IRC_CONFIG_HPP
