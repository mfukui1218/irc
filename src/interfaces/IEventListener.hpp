#ifndef IEVENT_LISTENER_HPP
#define IEVENT_LISTENER_HPP

#include "IServer.hpp"
#include "ISession.hpp"

class IEventListener
{
    public:
        virtual ~IEventListener() {}

        virtual void onConnected(ISession *session, IServer *server) = 0;
        virtual void onCommunication(ISession *session, IServer *server) = 0;
        virtual void onDisconnected(ISession *session, IServer *server) = 0;
};

#endif // IEVENT_LISTENER_HPP
