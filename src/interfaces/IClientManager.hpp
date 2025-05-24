#ifndef ICLIENT_MANAGER_HPP
#define ICLIENT_MANAGER_HPP

#include "IClient.hpp"

class IClientManager
{
    public:
        virtual ~IClientManager() {}

        /// @brief クライアントの登録
        /// @param client クライアント
        virtual void registerClient(IClient *client) = 0;

        /// @brief クライアントの検索（オーバーライド想定）
        /// @param nickName クライアント名
        /// @return クライアントへのポインタ
        virtual IClient *findClient(const std::string &nickName) = 0;

        /// @brief クライアントの登録解除
        /// @param client クライアント
        virtual void unregisterClient(IClient *client) = 0;
};

#endif // ICLIENT_MANAGER_HPP
