#ifndef SESSION_HPP
#define SESSION_HPP

#include "IClient.hpp"
#include <poll.h>
#include <sstream>

class Session
{
    private:
        struct pollfd *_pollfd;
        IClient *_client;

    public:
        Session();
        Session(const Session &other);
        virtual ~Session();
        Session operator=(const Session &rhs);

        Session(struct pollfd *_pollfd, IClient *client);

        std::istringstream *createStringStream(void); // 実装の場所ここじゃない感 
};

#endif // SESSION_HPP
