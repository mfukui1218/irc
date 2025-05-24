#ifndef ICLIENT_HPP
#define ICLIENT_HPP

#include "IClientManager.hpp"
#include <string>

class IClient
{
    public:
        virtual ~IClient() {}
        virtual std::string getNickName(void) const = 0;
        // その他のクライアントが持つべきメソッド
};

#endif // ICLIENT_HPP
