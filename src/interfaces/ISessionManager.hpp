#ifndef ISESSION_MANAGER_HPP
#define ISESSION_MANAGER_HPP

#include "ISession.hpp"
#include <string>
#include <vector>

class ISessionManager
{
    public:
        virtual ~ISessionManager() {}
        virtual void addSession(ISession *session) = 0;
        virtual void removeSession(ISession *session) = 0;
        virtual const ISession &findSession(const std::string clientName) = 0;
};

#endif // ISESSION_MANAGER_HPP
