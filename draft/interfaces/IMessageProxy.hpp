#ifndef IMESSAGE_PROXY_HPP
#define IMESSAGE_PROXY_HPP

#include "IClient.hpp"

class IMessageProxy
{
    public:
        virtual ~IMessageProxy() {}

        virtual void sendMessage(const IClient *receiver, const std::string &msg) = 0;
        virtual void flushMessage(const IClient *receiver, const std::string &msg) = 0;
};

#endif // IMESSAGE_PROXY_HPP
