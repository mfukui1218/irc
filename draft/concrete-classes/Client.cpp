#include "Client.hpp"

/// @brief
Client::Client() {}

/// @brief
/// @param other
Client::Client(const Client &other)
{
    // copy constructor
}

/// @brief
Client::~Client() {}

/// @brief
/// @param rhs
/// @return
Client Client::operator=(const Client &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

Client::Client(std::string nickName)
    :  _nickName(nickName)
{}

std::string Client::getNickName(void) const
{
    return _nickName;
}

/// @brief たぶん、ウェルカアムメッセージ送信
void Client::onConnected(Session *, IRCServer *) {}

/// @brief コマンド実行
void Client::onCommunication(Session *, IRCServer *) {}

void Client::onDisconnected(Session *, IRCServer *) {}
