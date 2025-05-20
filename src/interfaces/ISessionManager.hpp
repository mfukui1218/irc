#ifndef ISESSION_MANAGER_HPP
#define ISESSION_MANAGER_HPP

// #include "Session.hpp"
#include <vector>
#include <string>

class Session;
class ISessionManager
{
    public:
        virtual ~ISessionManager() {}
        virtual void addSession(Session *session) = 0;
        virtual void removeSession(Session *session) = 0;
        virtual std::vector<Session *> findSessions(std::string channelName) = 0;
};

#endif // ISESSION_MANAGER_HPP
