#ifndef ICHANNEL_HPP
#define ICHANNEL_HPP

#include "IClient.hpp"
#include <string>

class IChannel
{
    public:
        virtual ~IChannel() {}
        virtual std::string getName(void) const = 0;
        virtual bool canSendMessage(IClient *client) = 0;
        virtual void join(IClient *client) = 0;
};

#endif // ICHANNEL_HPP
