#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include "IChannelManager.hpp"
#include "IClientManager.hpp"
#include "IEventListener.hpp"
#include "IRCConfig.hpp"
#include "IMessageProxy.hpp"

class IRCServer : public IEventListener
{
    private:
        IChannelManager *_channelManager;
        IClientManager *_clientManager;
        IRCConfig *_config;
        IMessageProxy *_messageProxy;

        IRCServer();

    public:
        IRCServer(IChannelManager &channelManager,
                  IClientManager &clientManager,
                  IRCConfig &config,
                  IMessageProxy &messageProxy);
        IRCServer(const IRCServer &other);
        ~IRCServer();
        IRCServer operator=(const IRCServer &rhs);

        IChannelManager *getChannelManager(void) const;
        IClientManager *getClientManager(void) const;
        IRCConfig *getConfig(void) const;
        IMessageProxy *getMessageProxy(void) const;

        // Inherit IEventListener
        void onConnected(Session *session, IRCServer *server);
        void onCommunication(Session *session, IRCServer *server);
        void onDisconnected(Session *session, IRCServer *server);
};

#endif // IRCSERVER_HPP
