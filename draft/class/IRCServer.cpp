#include "IRCServer.hpp"

/// @brief
IRCServer::IRCServer()
    : _channelManager(NULL)
    , _clientManager(NULL)
    , _config(NULL)
{}

/// @brief
/// @param other
IRCServer::IRCServer(const IRCServer &other)
{
    // copy constructor
}

/// @brief
IRCServer::~IRCServer() {}

/// @brief
/// @param rhs
/// @return
IRCServer IRCServer::operator=(const IRCServer &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

IRCServer::IRCServer(IChannelManager &channelManager,
                     IClientManager &clientManager,
                     IRCConfig &config,
                     IMessageProxy &messageProxy)
    : _channelManager(&channelManager)
    , _clientManager(&clientManager)
    , _config(&config)
    , _messageProxy(&messageProxy)
{}

IChannelManager *IRCServer::getChannelManager(void) const
{
    return (NULL);
}

IClientManager *IRCServer::getClientManager(void) const
{
    return (NULL);
}

IRCConfig *IRCServer::getConfig(void) const
{
    return (NULL);
}

IMessageProxy *IRCServer::getMessageProxy(void) const
{
    return (NULL);
}

void IRCServer::onConnected(Session *, IRCServer *) {}
void IRCServer::onCommunication(Session *, IRCServer *) {}
void IRCServer::onDisconnected(Session *, IRCServer *) {}
