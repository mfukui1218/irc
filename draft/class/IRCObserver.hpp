#ifndef IRCOBSERVER_HPP
#define IRCOBSERVER_HPP

#include "IEventListener.hpp"
#include <vector>

class IRCObserver
{
    private:
        std::vector<IEventListener *> eventListenerList;

        IRCObserver();
        IRCObserver(const IRCObserver &other);
        virtual ~IRCObserver();
        IRCObserver operator=(const IRCObserver &rhs);

    public:
        static IRCObserver *getInstance();
        static void destroyInstance();

        void addEventListener(IEventListener *listener);
        void removeEventListener(IEventListener *listener);
        void startObserve();
        void stopObserve();
};

#endif // IRCOBSERVER_HPP
