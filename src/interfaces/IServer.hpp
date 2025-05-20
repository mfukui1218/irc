#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "IChannelManager.hpp"
#include "IConfig.hpp"
#include "IMessageProxy.hpp"
#include "ISessionManager.hpp"

class IServer
{
    public:
        virtual ~IServer() {}
        virtual const ISessionManager &getSessionManager(void) const = 0;
        virtual const IMessageProxy &getMessageProxy(void) const = 0;
        virtual const IChannelManager &getChannelManager(void) const = 0;
        virtual const IConfig &getConfig(void) const = 0;
};

#endif // ISERVER_HPP
