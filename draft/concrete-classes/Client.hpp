#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "IClient.hpp"
#include "IRCServer.hpp"
#include "IEventListener.hpp"
#include "Session.hpp"
#include <string>

class Client : public IClient, public IEventListener
{
    private:
        std::string _nickName;

    public:
        Client();
        Client(const Client &other);
        virtual ~Client();
        Client operator=(const Client &rhs);

        Client(std::string nickName);
        std::string getNickName(void) const;

        // inherit IEventListener
        void onConnected(Session *session, IRCServer *server);
		void onCommunication(Session *session, IRCServer *server);
		void onDisconnected(Session *session, IRCServer *server);
};

#endif // CLIENT_HPP
