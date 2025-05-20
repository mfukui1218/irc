#ifndef IMESSAGE_PROXY_HPP
#define IMESSAGE_PROXY_HPP

#include "IClient.hpp"

class IMessageProxy
{
    public:
        virtual ~IMessageProxy() {}

        /// @brief
        /// メッセージを送信する。バッファがいっぱいになるかflushMessageが呼ばれるまで実際に送信はしない。
        /// @param receiver メッセージの受け取り手
        /// @param msg メッセージ（生のテキストデータ）
        virtual void sendMessage(const IClient *receiver, const std::string &msg) = 0;
        virtual void flushMessage(const IClient *receiver, const std::string &msg) = 0;
};

#endif // IMESSAGE_PROXY_HPP
