#ifndef ICHANNEL_MANAGER_HPP
#define ICHANNEL_MANAGER_HPP

#include "IChannel.hpp"
#include <vector>

class IChannelManager
{
    private:
        std::vector<IChannel *> channelList;

    public:
        virtual ~IChannelManager() {}

        /// @brief チャンネルをマネージャーに登録する
        /// @param channel チャンネル
        virtual void registerChannel(IChannel *channel) = 0;

        /// @brief チャンネル名からチャンネルを見つける
        /// @param channelName チャンネル名
        /// @return チャンネル
        virtual IChannel *findChannel(const std::string &channelName) const = 0;

        /// @brief チャンネルを管理から外す。
        /// @param channel チャンネル
        virtual void unregisterChannel(IChannel *channel) = 0;
};

#endif // ICHANNEL_MANAGER_HPP
