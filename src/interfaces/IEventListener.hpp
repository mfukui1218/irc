#ifndef IEVENT_LISTENER_HPP
# define IEVENT_LISTENER_HPP

// #include "Session.hpp"
// #include "IRCServer.hpp"

class Session;
class IRCServer;

class IEventListener
{
	public:
		virtual ~IEventListener() {}

		/// @brief 接続時に必要な引数を指定する。まだ具体的には決まっていない。サーバーが良さそうだと思っている。
		virtual void onConnected(Session *session, IRCServer *server) = 0;
		virtual void onCommunication(Session *session, IRCServer *server) = 0;
		virtual void onDisconnected(Session *session, IRCServer *server) = 0;
};

#endif // IEVENT_LISTENER_HPP
