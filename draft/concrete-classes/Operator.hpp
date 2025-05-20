#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "IClient.hpp"
#include "IEventListener.hpp"
#include "Session.hpp"
#include "IRCServer.hpp"
#include <string>

class Operator : public IClient, public IEventListener
{
    private:
        std::string _nickName;

    public:
        Operator();
        Operator(const Operator &other);
        virtual ~Operator();
        Operator operator=(const Operator &rhs);

        Operator(std::string nickName);
        std::string getNickName(void) const;
        void onConnected(Session *session, IRCServer *server);
		void onCommunication(Session *session, IRCServer *server);
		void onDisconnected(Session *session, IRCServer *server);
};

#endif // OPERATOR_HPP
