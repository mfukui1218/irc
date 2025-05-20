#include "Session.hpp"

/// @brief
Session::Session()
    : _pollfd(NULL)
    , _client(NULL)
{}

/// @brief
/// @param other
Session::Session(const Session &other)
{
    // copy constructor
}

/// @brief
Session::~Session() {}

/// @brief
/// @param rhs
/// @return
Session Session::operator=(const Session &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

Session::Session(struct pollfd *fd, IClient *client)
    : _pollfd(fd)
    , _client(client)
{}

std::istringstream *Session::createStringStream(void) // 実装の場所ここじゃない感
{
    return NULL;
}
