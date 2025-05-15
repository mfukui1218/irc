#include "Operator.hpp"

/// @brief
Operator::Operator() {}

/// @brief
/// @param other
Operator::Operator(const Operator &other)
{
    // copy constructor
}

/// @brief
Operator::~Operator() {}

/// @brief
/// @param rhs
/// @return
Operator Operator::operator=(const Operator &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

Operator::Operator(std::string nickName)
    : _nickName(nickName)
{}

std::string Operator::getNickName(void) const
{
    return _nickName;
}

void Operator::onConnected(Session *session, IRCServer *server) {}

void Operator::onCommunication(Session *session, IRCServer *server) {}

void Operator::onDisconnected(Session *session, IRCServer *server) {}
