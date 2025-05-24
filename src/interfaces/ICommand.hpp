#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "IServer.hpp"
#include "ISession.hpp"

class ICommand
{
    public:
        virtual ~ICommand() {}
        virtual void execute(ISession *session, IServer *server) const = 0;
};

#endif // ICOMMAND_HPP
