#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

#include "Session.hpp"
#include "IRCServer.hpp"

class ICommand
{
	private:
	protected:
	public:
		virtual ~ICommand() {}
		virtual void execute(Session *session, IRCServer *server) = 0;
};

#endif // ICOMMAND_HPP
