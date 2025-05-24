#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include "Client.hpp"
#include "Server.hpp"
#include "IChannel.hpp"

class Client;  // Forward declaration

class Channel : public IChannel
{
private:
    std::string _channelName;
    std::string _topic;
    std::set<Client*> _clients;
    std::set<Client*> _operators;
    std::string _key;  // +k mode (password)
    std::set<Client*> _invited;
    int _userLimit;    // +l mode
    bool _inviteOnly;  // +i mode
    bool _topicLocked; // +t mode

public:
    Channel(const std::string& name);
    ~Channel();

    // IChannel interface
    std::string getName() const { return _channelName; }
    void setTopicLock(bool enabled) { _topicLocked = enabled; }
    bool isTopicLocked() const { return _topicLocked; }
    void clearUserLimit() { _userLimit = 0; }
    void addClient(Client* client);
    void removeClient(Client* client);
    bool hasClient(Client* client) const;

    void addOperator(Client& client);
    void removeOperator(Client& client);
    bool isOperator(Client* client) const;

    std::string getTopic() const;
    void setTopic(const std::string& topic);

    void setKey(const std::string& key);
    std::string getKey() const;
    void removeKey();

    void setUserLimit(int limit);
    int getUserLimit() const;

    void setInviteOnly(bool invite);
    bool isInviteOnly() const;

    bool hasMember(const Client& client) const;

    std::string getChannelName() const;
	void broadcast(const std::string& message, Client* except = NULL);

	std::set<Client*> getClients() const
	{
		return _clients;
	}

    bool isLimitEnabled() const;
    int getUserCount() const;
    void applyMode(const std::string& mode, std::stringstream& argStream, const Client& client, Server& server);
    void removeMode(const std::string& mode);
    void set_channelpass(bool adding, const std::string& pass, const Client& client);
    void set_userlimit(bool adding, int limit, const Client& client);
    void set_operator(bool adding, const std::string& nickname, Client& issuer, Server& server);
    void invite(Client* c) { _invited.insert(c); }
	bool isInvited(Client* c) const {
		return _invited.count(c) > 0;
	}
	void removeInvite(Client* c) {
		_invited.erase(c);
	}
};

#endif
