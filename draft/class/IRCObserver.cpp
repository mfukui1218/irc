#include "IRCObserver.hpp"
#include <cstddef>

/// @brief
IRCObserver::IRCObserver() {}

/// @brief
/// @param other
IRCObserver::IRCObserver(const IRCObserver &other)
{
    // copy constructor
}

/// @brief
IRCObserver::~IRCObserver() {}

/// @brief
/// @param rhs
/// @return
IRCObserver IRCObserver::operator=(const IRCObserver &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

IRCObserver *IRCObserver::getInstance()
{
    return (NULL);
}

void IRCObserver::destroyInstance() {}

void IRCObserver::addEventListener(IEventListener *listener) {}

void IRCObserver::removeEventListener(IEventListener *listener) {}

void IRCObserver::startObserve() {}

void IRCObserver::stopObserve() {}
