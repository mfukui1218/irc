#ifndef ISESSION_HPP
#define ISESSION_HPP

#include "IClient.hpp"

class ISession
{
    public:
        virtual ~ISession() {}
        virtual int getSockFd(void) const = 0;
        virtual const IClient &getClient(void) const = 0;
};

#endif // ISESSION_HPP
